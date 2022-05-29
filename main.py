from tkinter import *
from tkinter.ttk import *
from socket import *
from os import *
from time import *

top = Tk()
top.geometry("450x300")

text1 = StringVar()
text2 = StringVar()

sv_sock_path_0 = "/tmp/sandbox/socket_0"
sockets = []


def get_data():
    txt1 = text1.get()
    txt2 = text2.get()
    txt = f'{txt1}&{txt2}&+'
    sfd_0 = sockets[0]
    print(f'sending {txt} to {sfd_0}')
    sfd_0.sendall(txt.encode())


def loop():
    # label for first number
    first_number = Label(top, text="First number")
    first_number.place(x=40, y=60)

    # label for second number
    second_number = Label(top, text="Second number")
    second_number.place(x=40, y=100)
    # entry for first number
    first_number_area = Entry(top, width=30, textvariable=text1)
    first_number_area.place(x=150, y=60)
    # entry for second number
    second_number_area = Entry(top, width=30, textvariable=text2)
    second_number_area.place(x=150, y=100)
    # submit button
    submit_button = Button(top,
                           text="Get Result:", command=get_data)
    submit_button.place(x=40, y=130)
    top.mainloop()


def connect_socket(sock_path):
    sock = socket(AF_UNIX, SOCK_STREAM)

    try:
        print(f'Connecting to {sock} at {sock_path}')
        sock.connect(sock_path)
    except error as msg:
        print(msg)
        delay = 3
        print(f'Retrying socket connection for {sock_path} in {delay} seconds')
        sleep(delay)
        return connect_socket(sock_path)

    return sock


if __name__ == '__main__':
    sockets.append(connect_socket(sv_sock_path_0))
    loop()
