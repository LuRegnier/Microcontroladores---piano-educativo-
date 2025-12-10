import tkinter as tk
from tkinter import simpledialog, messagebox
import rtmidi
import time 
import serial
import threading
import json
import os


PORTA_ARDUINO = 'COM9' 
BAUD_RATE = 9600  
ARQUIVO_MELODIAS = "melodias_piano.json"
TOLERANCIA_ACORDE = 100

try:
    ser = serial.Serial(PORTA_ARDUINO, BAUD_RATE, timeout=0.1)
    print(f"Conectado ao Arduino na {PORTA_ARDUINO}")
except:
    print("ERRO: Arduino não conectado ou porta errada.")

ARDUINO_MAPA = {
    "DO": 60, "DOS": 61, "RE": 62, "RES": 63,
    "MI": 64, "FA": 65, "FAS": 66, "SOL": 67,
    "SOLS": 68, "LA": 69, "LAS": 70, "SI": 71
}

MAPA_ARDUINO_INVERTIDO = {v: k for k, v in ARDUINO_MAPA.items()}

midiout = rtmidi.MidiOut()
if midiout.get_ports():
    midiout.open_port(0)
else:
    midiout.open_virtual_port("Pianinho Virtual")

NOTA_ON = 0x90
NOTA_OFF = 0x80

BOTOES_NOTAS = [
    ("Dó", 60), ("Dó#", 61), ("Ré", 62), ("Ré#", 63),
    ("Mi", 64), ("Fá", 65), ("Fá#", 66), ("Sol", 67),
    ("Sol#", 68), ("Lá", 69), ("Lá#", 70), ("Si", 71),
]

MAPA_TECLADO = {
    'd': 60, 'r': 61, 'f': 62, 't': 63, 'g': 64, 'h': 65, 
    'u': 66, 'j': 67, 'i': 68, 'k': 69, 'o': 70, 'l': 71
}


melodia_gravada = []
dic_melodias_gravadas = {}
teclas_config = {} 
teclas_pressionadas = []
gravando = False
tempo_inicio_gravacao = 0

modo_aluno = False
melodia_aluno = [] 
indice_nota_atual = 0
play = True

timer_espera = None
TEMPO_LIMITE = 5000

def carregar_melodias_do_arquivo():
    global dic_melodias_gravadas
    if os.path.exists(ARQUIVO_MELODIAS):

        with open(ARQUIVO_MELODIAS, "r", encoding="utf-8") as f:
            dic_melodias_gravadas = json.load(f)
        print(f"Músicas carregadas: {list(dic_melodias_gravadas.keys())}")
        
    else:
        print("Nenhum arquivo encontrado")

def salvar_melodias_no_arquivo():
    with open(ARQUIVO_MELODIAS, "w", encoding="utf-8") as f:
        json.dump(dic_melodias_gravadas, f, indent = 4)
    print("Arquivo salvo com sucesso!")


def abrir_seletor_musica():
    if not dic_melodias_gravadas:
        messagebox.showwarning("Aviso", "Nenhuma música salva.")
        return
    
    janela_modal = tk.Toplevel(janela)
    janela_modal.title("Escolha uma música")
    janela_modal.geometry("500x600")
    
    lbl = tk.Label(janela_modal, text="Qual música você quer tocar?", font=("Arial", 12, "bold"))
    lbl.pack(pady=10)

    frame_lista = tk.Frame(janela_modal)
    frame_lista.pack(fill="both", expand=True, padx=10, pady=10)

    for nome_musica in dic_melodias_gravadas:
        btn = tk.Button(frame_lista, text=f"{nome_musica}", font=("Arial", 10),bg="#eee",
                        command=lambda n=nome_musica, w=janela_modal: preparar_aula(n, w))
        btn.pack(fill="x", pady=2)

    btn_cancelar = tk.Button(janela_modal, text="Cancelar", command=janela_modal.destroy, fg="red")
    btn_cancelar.pack(pady=5)

def preparar_aula(nome_musica, janela_modal):
    global melodia_gravada
    
    melodia_gravada = dic_melodias_gravadas[nome_musica]
    
    print(f"Música selecionada: {nome_musica}")
    janela_modal.destroy() 
    iniciar_modo_aluno() 

def iniciar_modo_aluno():
    global modo_aluno, melodia_aluno, indice_nota_atual, play
    
    if not melodia_gravada:
        return 

    melodia_aluno = [] 

    for evento in melodia_gravada:
        if evento['tipo'] == 'ON':
            melodia_aluno.append(evento)
    
    modo_aluno = True
    indice_nota_atual = 0
    label_status.config(text="Modo aluno iniciado", fg="black")
    
    mostrar_dica_atual()

def mostrar_dica_atual():
    global timer_espera
    if timer_espera:
        janela.after_cancel(timer_espera)
        
    if indice_nota_atual < len(melodia_aluno):
        
        tempo_referencia = melodia_aluno[indice_nota_atual]['tempo']
        
        for i in range(indice_nota_atual, len(melodia_aluno)):
            proxima_nota = melodia_aluno[i]
            diferenca_tempo = proxima_nota['tempo'] - tempo_referencia
            
            if diferenca_tempo <= TOLERANCIA_ACORDE:
                nota_alvo = proxima_nota['nota']
                nome_nota = MAPA_ARDUINO_INVERTIDO[nota_alvo]

                enviar_comando_arduino(nome_nota, "YELLOW")
                print(f"Dica Acorde: {nome_nota} (Amarelo)")
                
                timer_espera = janela.after(TEMPO_LIMITE, lambda n=nome_nota: timeout_nota(n))
            else:
                break
    else:
        finalizar_modo_aluno()
        
def timeout_nota(nome_nota):
    enviar_comando_arduino(nome_nota, "RED")

def enviar_comando_arduino(nota_txt, estado):
    if ser:
        cmd = f"{nota_txt}_{estado}\n"
        ser.write(cmd.encode())
        
def pausar_musica():
    global play
    if play == True:    
        play = False
        label_status.config(text="PAUSADO")
    else:
        play = True
    
def finalizar_modo_aluno():
    global modo_aluno
    modo_aluno = False
    label_status.config(text="Musica concluida", fg="green")
    print("Fim do modo aluno")


def millis():
    return round(time.time() * 1000)

def thread_conectar_arduino():
    global indice_nota_atual, timer_espera
    print("Thread Arduino OK")
    while True:
        if ser and ser.in_waiting:

            linha = ser.readline().decode('utf-8').strip()
            if "_" in linha:
                partes = linha.split("_")
                
                if len(partes) == 2:  
                    nome_nota = partes[0]
                    estado = partes[1]
                    
                    if nome_nota in ARDUINO_MAPA:
                        nota_midi = ARDUINO_MAPA[nome_nota]
                        
                        if modo_aluno and estado == "ON":
                            nota_correta = melodia_aluno[indice_nota_atual]['nota']
                            
                            if nota_midi == nota_correta:
                                if timer_espera:
                                    janela.after_cancel(timer_espera)
                                
                                # Acertou
                                janela.after(0, lambda: tocar_nota_midi(nota_midi, 100))
                                janela.after(0, lambda n=nome_nota: enviar_comando_arduino(n, "NOYELLOW"))
                                print("acertou a nota")
                                indice_nota_atual += 1
                                janela.after(100, mostrar_dica_atual)
                                print()
                            elif nota_midi != nota_correta:
                                # Errou
                                janela.after(0, lambda n=nota_correta: enviar_comando_arduino(n, "RED"))
                                janela.after(0, lambda: tocar_nota_midi(nota_midi, 100))
                                print("Errou a nota")

                        elif not modo_aluno:
                            if estado == "ON":
                                janela.after(0, lambda: tocar_nota_midi(nota_midi, 100))
                                janela.after(0, lambda: registrar_evento(nota_midi, 'ON'))
                            elif estado == "OFF":
                                janela.after(0, lambda: desligar_nota_midi(nota_midi))
                                janela.after(0, lambda: registrar_evento(nota_midi, 'OFF'))
         

def tocar_nota_midi(nota, velocidade):
    midiout.send_message([NOTA_ON, nota, velocidade])
    if nota in teclas_config:
        canvas.itemconfig(teclas_config[nota]['id'], fill = "#7cd2a8")
        label_status.config(text=f"Tocando MIDI: {nota}") 

def desligar_nota_midi(nota):
    midiout.send_message([NOTA_OFF, nota, 0])
    if nota in teclas_config:
        cor = teclas_config[nota]['cor_original']
        canvas.itemconfig(teclas_config[nota]['id'], fill = cor)
        label_status.config(text = "Pronto")

def registrar_evento(nota, tipo):
    if gravando:
        tempo_atual = millis()
        delta_tempo = tempo_atual - tempo_inicio_gravacao
        evento = {
            'tempo': delta_tempo,
            'nota': nota,
            'tipo': tipo
        }
        melodia_gravada.append(evento)
        print(f"Rec: {evento}")

def btn_iniciar_gravacao():
    global gravando, tempo_inicio_gravacao, melodia_gravada
    gravando = True
    melodia_gravada = [] 
    tempo_inicio_gravacao = millis() 
    label_status.config(text="GRAVANDO", fg="red")
    print("Gravação Iniciada")

def btn_parar_gravacao():
    global gravando
    gravando = False
    
    if melodia_gravada:
        nome_musica = simpledialog.askstring("Salvar Música", "Digite o nome da música:")
        
        if nome_musica:
            dic_melodias_gravadas[nome_musica] = melodia_gravada
            
            salvar_melodias_no_arquivo()
            label_status.config(text=f"Musica '{nome_musica}' Salva!", fg="blue")
        else:
            label_status.config(text="Gravação descartada (sem nome).", fg="black")
    else:
        label_status.config(text="Nada foi gravado.", fg="black")

def btn_reproduzir():
    if not melodia_gravada:
        label_status.config(text="Nenhuma música gravada")
        return

    label_status.config(text="Reproduzindo", fg="blue")
    for evento in melodia_gravada:
        tempo_para_esperar = evento['tempo']
        
        nota = evento['nota']
        tipo = evento['tipo']
        
        if tipo == 'ON':
            janela.after(tempo_para_esperar, lambda n=nota: tocar_nota_midi(n, 100))
        elif tipo == 'OFF':
            janela.after(tempo_para_esperar, lambda n=nota: desligar_nota_midi(n))
            
    ultimo_tempo = melodia_gravada[-1]['tempo']
    janela.after(ultimo_tempo + 500, atualiza_label_final)

def atualiza_label_final():
    label_status.config(text ="Fim da reprodução", fg = "blue")
    
def apertar_tecla(evento):
    tecla = evento.char.lower()
    if tecla in MAPA_TECLADO and tecla not in teclas_pressionadas:
        nota = MAPA_TECLADO[tecla]
        tocar_nota_midi(nota, 100)
        teclas_pressionadas.append(tecla)
        registrar_evento(nota, 'ON')

def soltar_tecla(evento):
    tecla = evento.char.lower()
    if tecla in MAPA_TECLADO and tecla in teclas_pressionadas:
        nota = MAPA_TECLADO[tecla]
        desligar_nota_midi(nota)
        teclas_pressionadas.remove(tecla)
        registrar_evento(nota, 'OFF')


janela = tk.Tk()
janela.title("Piano")
janela.bind("<KeyPress>", apertar_tecla)
janela.bind("<KeyRelease>", soltar_tecla)

carregar_melodias_do_arquivo()

frame_gravador = tk.Frame(janela, bg="#ddd", bd=2, relief="groove")
frame_gravador.pack(pady=10, fill="x")

botao_gravar = tk.Button(frame_gravador, text="Gravar", bg="#ffcccc", command=btn_iniciar_gravacao)
botao_parar_gravacao = tk.Button(frame_gravador, text="Salvar Gravação", command=btn_parar_gravacao)
botao_reproduzir = tk.Button(frame_gravador, text="Reproduzir", bg="#ccffcc", command=btn_reproduzir)

botao_aluno = tk.Button(frame_gravador, text="Modo Aluno", bg= "yellow", command=abrir_seletor_musica)

botao_gravar.pack(side="left", padx=5, pady=5)
botao_parar_gravacao.pack(side="left", padx=5, pady=5)
botao_reproduzir.pack(side="left", padx=5, pady=5)
botao_aluno.pack(side="left", padx=5, pady=5)

canvas = tk.Canvas(janela, width=800, height=200, bg="#202020")
canvas.pack(padx=20, pady=10)

label_status = tk.Label(janela, text="piano", font=("Helvetica", 12))
label_status.pack(pady=5)

largura = 50
altura = 120
margem = 70
y = 30
espaco = 5


def desenhar_tecla(x, nome, nota, cor, cor_lbl):
    rect = canvas.create_rectangle(x, y, x+largura, y+altura, fill=cor, outline="black", width=2)
    canvas.create_text(x+largura/2, y+altura+15, text=nome, fill=cor_lbl)
    teclas_config[nota] = {"id": rect, "cor_original": cor}

i = 0
for nome, nota in BOTOES_NOTAS:
    x = margem + i * (largura + espaco)
    if "#" in nome:
        cor_tecla = "#000000"
        cor_contorno = "#ffffff"
    else:
        cor_tecla = "#fdfdfd"
        cor_contorno = "#ffffff"
    desenhar_tecla(x, nome, nota, cor_tecla, cor_contorno)
    i+=1
    
t = threading.Thread(target=thread_conectar_arduino)
t.daemon = True 
t.start()

janela.mainloop()