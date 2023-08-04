import pyautogui
import tkinter as tk

# Função para obter o tamanho da tela
def get_screen_size():
    root = tk.Tk()
    root.withdraw()
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()
    root.destroy()
    return screen_width, screen_height

# Função para mover o mouse com o efeito do logo do DVD
def move_mouse_logo_effect():
    screen_width, screen_height = get_screen_size()
    mouse_x, mouse_y = pyautogui.position()
    direction_x, direction_y = 1, 1  # Direções iniciais (1: direita, -1: esquerda)
    speed = 10  # Velocidade do movimento

    while True:
        mouse_x += direction_x * speed
        mouse_y += direction_y * speed

        # Verificar colisão nas quinas da tela
        if mouse_x < 0 or mouse_x > screen_width:
            direction_x *= -1  # Mudar a direção horizontal
            mouse_x += direction_x * speed

        if mouse_y < 0 or mouse_y > screen_height:
            direction_y *= -1  # Mudar a direção vertical
            mouse_y += direction_y * speed

        pyautogui.moveTo(mouse_x, mouse_y)
        pyautogui.sleep(0.01)

# Iniciar o movimento do mouse com o efeito do logo do DVD
move_mouse_logo_effect()
