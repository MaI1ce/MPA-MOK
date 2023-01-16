from tkinter import *
from tkinter import ttk
from tkinter import filedialog as fd
import sys
sys.path.insert(0, '..\..\lib')
from mpamok import *
#from PyRBinLWE.lib.mpamok import *

#######################################################################

textwidth = 80

class MainAppWindow:
    def __init__(self):
        self.encryptor = RBinLWE()
        self.encryptor.init()
        self.init_polynomial_a = Polynomial()
        self.Keys = None
        self.root = Tk()
        self.root.title("MPA-MOK-2022")

        #######################################################################

        self.mainframe = ttk.Frame(self.root, padding="3 3 3 3")
        self.mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        #######################################################################

        self.KeyTextFrame = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.KeyTextFrame.grid(column=1, row=0, rowspan=3, sticky=(N, W, E, S))
        self.tKey = Text(self.KeyTextFrame, width=textwidth - 15, height=10, wrap='char')
        self.tKey.grid(column=0, row=0, sticky=(N, W, E, S))
        #self.tKey['state'] = 'disabled'
        self.ys_key = ttk.Scrollbar(self.KeyTextFrame, orient='vertical', command=self.tKey.yview)
        self.tKey['yscrollcommand'] = self.ys_key.set
        self.ys_key.grid(column=1, row=0, sticky=(N, S))

        # add event handlers
        self.BtnKeyGen = ttk.Button(self.mainframe, text="Generate new keys", command=self.generate_new_keys)
        self.BtnKeyGen.grid(column=0, row=0, sticky=(N, W, E, S))

        self.BtnImportKey = ttk.Button(self.mainframe, text="Import keys from file", command=self.import_keys_from_file)
        self.BtnImportKey.grid(column=0, row=1, sticky=(N, W, E, S))

        self.BtnSaveKey = ttk.Button(self.mainframe, text="Save keys to file", command=self.save_keys_to_file)
        self.BtnSaveKey.grid(column=0, row=2, sticky=(N, W, E, S))

        # ttk.Label(mainframe, text="in proces").grid(column=1, row=0, sticky=W)

        #######################################################################

        self.BtnEncrypt = ttk.Button(self.mainframe, text="Encrypt text", command=None)
        self.BtnEncrypt.grid(column=0, row=3, sticky=(N, W, E, S))

        self.input_frame = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.input_frame.grid(column=1, row=3, sticky=(N, W, E, S))
        ttk.Label(self.input_frame, text="Input text").grid(column=0, row=0, sticky=W)
        self.input_text = StringVar()
        self.input_text_entry = ttk.Entry(self.input_frame, width=85, textvariable=self.input_text)
        self.input_text_entry.grid(column=0, row=1, sticky=(W, E))

        self.output_frame1 = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.output_frame1.grid(row=4, columnspan=2)
        ttk.Label(self.output_frame1, text="Encryption output text").grid(row=0, sticky=W)
        self.EncryptOutputText = Text(self.output_frame1, width=textwidth, height=10, wrap='char')
        self.EncryptOutputText.grid(column=0, row=1, sticky=(N, W, E, S))
        #self.EncryptOutputText['state'] = 'disabled'
        self.ysEnc = ttk.Scrollbar(self.output_frame1, orient='vertical', command=self.EncryptOutputText.yview)
        self.EncryptOutputText['yscrollcommand'] = self.ysEnc.set
        self.ysEnc.grid(column=1, row=1, sticky=(N, S))

        #######################################################################

        self.BtnDecrypt = ttk.Button(self.mainframe, text="Decrypt text", command=None)
        self.BtnDecrypt.grid(column=0, row=5, sticky=(N, W, E, S), ipady=10)

        self.output_frame2 = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.output_frame2.grid(row=6, columnspan=2)
        ttk.Label(self.output_frame2, text="Decryption output text").grid(row=0, sticky=W)
        self.DecryptOutputText = Text(self.output_frame2, width=textwidth, height=10, wrap='char')
        self.DecryptOutputText.grid(column=0, row=1, sticky=(N, W, E, S))
        #self.DecryptOutputText['state'] = 'disabled'
        self.ysDec = ttk.Scrollbar(self.output_frame2, orient='vertical', command=self.DecryptOutputText.yview)
        self.DecryptOutputText['yscrollcommand'] = self.ysEnc.set
        self.ysDec.grid(column=1, row=1, sticky=(N, S))

        #######################################################################

        self.EncryptFileBtn = ttk.Button(self.mainframe, text="Encrypt file", command=None)
        self.EncryptFileBtn.grid(column=0, row=7, sticky=(N, W, E, S), ipady=10)

        self.DecryptFileBtn = ttk.Button(self.mainframe, text="Decrypt file", command=None)
        self.DecryptFileBtn.grid(column=0, row=8, sticky=(N, W, E, S), ipady=10)

        for child in self.mainframe.winfo_children():
            child.grid_configure(padx=5, pady=5)

        # feet_entry.focus()
        # root.bind("<Return>", calculate)

        self.root.mainloop()

    def generate_new_keys(self):
        self.init_polynomial_a.random_init()
        self.Keys = self.encryptor.key_generator(self.init_polynomial_a)
        key_str = self.Keys.print_string()#.decode('latin-1')
        self.tKey.delete('1.0', END)
        #self.tKey['state'] = 'enabled'
        self.tKey.insert(INSERT, key_str)
        #self.tKey['state'] = 'disabled'

    def open_file_wrie(self):
        filename = fd.askopenfilename()
        f = open(filename,'wb')

    def save_keys_to_file(self):
        #filename = fd.askopenfilename()
        f_public = open('key.public', 'wb')
        f_private = open('key.private', 'wb')
        public_poly_size = self.Keys.public_key.get_poly_mod()

        f_public.write(public_poly_size.to_bytes(2, 'big'))
        f_public.write(b'\n'+self.Keys.public_key.to_string())

        f_private.write(public_poly_size.to_bytes(2,'big'))
        f_private.write(b'\n' + self.Keys.private_key.to_string())

    def import_keys_from_file(self):
        #filename = fd.askopenfilename()
        f_public = open('key.public', 'rb')
        f_private = open('key.private', 'rb')
        self.Keys = KeyRing()

        public_key_poly_size = int.from_bytes(f_public.read(2), 'big')
        f_public.read(1)
        pub_key_str = f_public.read()
        self.Keys.public_key.init_str(pub_key_str,public_key_poly_size)

        private_key_poly_size = int.from_bytes(f_private.read(2), 'big')
        f_public.read(1)
        prive_key_str = f_private.read()
        self.Keys.private_key.init_str(prive_key_str, private_key_poly_size)

        key_str = self.Keys.print_string()#.decode('latin-1')
        self.tKey.delete('1.0', END)
        #self.tKey['state'] = 'enabled'
        self.tKey.insert(INSERT, key_str)



    

if __name__ == "__main__":
    app_win = MainAppWindow()

















