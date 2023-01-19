from tkinter import *
from tkinter import ttk
from tkinter import filedialog as fd
import sys
import os
import copy
sys.path.insert(0, '..\..\lib')
#sys.path.insert(0, 'F:\MPA-MOK\PyRBinLWE\lib')

from mpamok import *
#from PyRBinLWE.lib.mpamok import *

#######################################################################
global_var = None

textwidth = 80

class MainAppWindow:
    def __init__(self):
        self.encryptor = RBinLWE()
        self.encryptor.init()
        self.init_polynomial_a = Polynomial()
        self.Keys = None
        self.key_size = None
        self.encrypted_str = None
        self.decrypted_str = None
        self.root = Tk()
        self.root.title("MPA-MOK-2022")

        #######################################################################

        self.mainframe = ttk.Frame(self.root, padding="3 3 3 3")
        self.mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        #######################################################################

        # add event handlers
        self.BtnKeyFrame = ttk.Frame(self.mainframe)
        self.BtnKeyFrame.grid(row=0, column=0, sticky=W+E)
        
        self.BtnKeyGen = ttk.Button(self.BtnKeyFrame, text="Generate new keys", command=self.generate_new_keys)
        self.BtnKeyGen.grid(column=0, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.BtnImportKey = ttk.Button(self.BtnKeyFrame, text="Import keys from file", command=self.import_keys_from_file)
        self.BtnImportKey.grid(column=1, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.BtnSaveKey = ttk.Button(self.BtnKeyFrame, text="Save keys to file", command=self.save_keys_to_file)
        self.BtnSaveKey.grid(column=2, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.KeyTextFrame = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.KeyTextFrame.grid(row=1, columnspan=3,sticky=(N, W, E, S))
        ttk.Label(self.KeyTextFrame, text="Keys output").grid(row=0, sticky=W)
        self.tKey = Text(self.KeyTextFrame, width=textwidth, height=10, wrap='char')
        self.tKey.grid(column=0, row=1, sticky=(N, W, E, S))
        #self.tKey['state'] = 'disabled'
        self.ys_key = ttk.Scrollbar(self.KeyTextFrame, orient='vertical', command=self.tKey.yview)
        self.tKey['yscrollcommand'] = self.ys_key.set
        self.ys_key.grid(column=1, row=1, sticky=(N, S))

        #######################################################################
        self.BtnEncryptFrame = ttk.Frame(self.mainframe)
        self.BtnEncryptFrame.grid(row=2, column=0, sticky=W+E)
        
        self.BtnEncrypt = ttk.Button(self.BtnEncryptFrame, text="Encrypt text", command=self.encrypt_txt)
        self.BtnEncrypt.grid(column=0, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.EncryptFileBtn = ttk.Button(self.BtnEncryptFrame, text="Encrypt file", command=self.encrypt_file)
        self.EncryptFileBtn.grid(column=1, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.input_frame = ttk.Frame(self.BtnEncryptFrame, padding="3 3 3 3")
        self.input_frame.grid(column=2, row=0, sticky=(N, W, E, S))
        ttk.Label(self.input_frame, text="Input text").grid(column=0, row=0, sticky=W)
        self.input_text = StringVar()
        self.input_text_entry = ttk.Entry(self.input_frame, width=85, textvariable=self.input_text)
        self.input_text_entry.grid(column=0, row=1, columnspan=2, sticky=(W, E))

        self.output_frame1 = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.output_frame1.grid(row=3, columnspan=3,sticky=(N, W, E, S))
        ttk.Label(self.output_frame1, text="Encryption output text").grid(row=0, sticky=W)
        self.EncryptOutputText = Text(self.output_frame1, width=textwidth, height=10, wrap='char')
        self.EncryptOutputText.grid(column=0, row=1, sticky=(N, W, E, S))
        #self.EncryptOutputText['state'] = 'disabled'
        self.ysEnc = ttk.Scrollbar(self.output_frame1, orient='vertical', command=self.EncryptOutputText.yview)
        self.EncryptOutputText['yscrollcommand'] = self.ysEnc.set
        self.ysEnc.grid(column=1, row=1, sticky=(N, S))

        ####################################################################### 
        self.BtnDecryptFrame = ttk.Frame(self.mainframe)
        self.BtnDecryptFrame.grid(row=4, column=0, sticky=W+E)
        
        self.BtnDecrypt = ttk.Button(self.BtnDecryptFrame, text="Decrypt text", command=self.decrypt_txt)
        self.BtnDecrypt.grid(column=0, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.DecryptFileBtn = ttk.Button(self.BtnDecryptFrame, text="Decrypt file", command=self.decrypt_file)
        self.DecryptFileBtn.grid(column=1, row=0, ipadx=textwidth/2, ipady=10, sticky=(N, W, E, S))

        self.output_frame2 = ttk.Frame(self.mainframe, padding="3 3 3 3")
        self.output_frame2.grid(row=5, columnspan=3,sticky=(N, W, E, S))
        ttk.Label(self.output_frame2, text="Decryption output text").grid(row=0, sticky=W)
        self.DecryptOutputText = Text(self.output_frame2, width=textwidth, height=10, wrap='char')
        self.DecryptOutputText.grid(column=0, row=1, sticky=(N, W, E, S))
        #self.DecryptOutputText['state'] = 'disabled'
        self.ysDec = ttk.Scrollbar(self.output_frame2, orient='vertical', command=self.DecryptOutputText.yview)
        self.DecryptOutputText['yscrollcommand'] = self.ysEnc.set
        self.ysDec.grid(column=1, row=1, sticky=(N, S))

        #######################################################################


        for child in self.mainframe.winfo_children():
            child.grid_configure(padx=5, pady=5)

        # feet_entry.focus()
        # root.bind("<Return>", calculate)
        self.mainframe.columnconfigure(0, weight=1)
        self.mainframe.rowconfigure(1, weight=1)
        self.input_frame.columnconfigure(0, weight=1)
        self.input_frame.rowconfigure(1, weight=1)
        self.KeyTextFrame.columnconfigure(0, weight=1)
        self.KeyTextFrame.rowconfigure(0, weight=1)
        self.output_frame1.rowconfigure(0, weight=1)
        self.output_frame1.columnconfigure(0, weight=1)
        self.output_frame2.rowconfigure(0, weight=1)
        self.output_frame2.columnconfigure(0, weight=1)

        self.root.mainloop()

    def generate_new_keys(self):
        self.tKey.delete('1.0', END)
        try:
            self.init_polynomial_a.random_init()
            self.Keys = self.encryptor.key_generator(self.init_polynomial_a)
            key_str = self.Keys.print_bytes()#.decode('latin-1')
            #self.tKey['state'] = 'enabled'
            self.tKey.insert(INSERT, "Initial polynomial a:\n")
            self.tKey.insert(INSERT, self.init_polynomial_a.print_bytes() + b'\n')
            self.tKey.insert(INSERT, key_str)
        except:
            self.tKey.insert(INSERT, "Error: Unexpected error")
        #self.tKey['state'] = 'disabled'

    def save_keys_to_file(self):
        filetype1 = (
            ('public key files', '*.public'),
            ('All files', '*.*')
        )
        filetype2 = (
            ('private key files', '*.private'),
            ('All files', '*.*')
        )
        if self.Keys != None:
            f_public = None
            f_private = None
            try:

                filename_public = fd.asksaveasfilename(
                    title='Save public key file',
                    initialdir='.',
                    filetypes=filetype1,
                    defaultextension='.public')
                f_public = open(filename_public, 'wb')

                filename_private = fd.asksaveasfilename(
                    title='Save private key file',
                    initialdir='.',
                    filetypes=filetype2,
                    defaultextension='.private')
                f_private = open(filename_private, 'wb')

                public_poly_size = self.Keys.public_key.get_poly_mod()

                f_public.write(public_poly_size.to_bytes(2, 'big'))
                f_public.write(b'\n'+self.Keys.public_key.to_bytes())
                f_public.write(self.init_polynomial_a.to_bytes())

                f_private.write(public_poly_size.to_bytes(2,'big'))
                f_private.write(b'\n' + self.Keys.private_key.to_bytes())

                self.tKey.delete('1.0', END)
                # self.tKey['state'] = 'enabled'
                self.tKey.insert(INSERT, 'Keys are saved in files: \n' + filename_public + '\n' + filename_private)
            finally:
                if f_public != None:
                    f_public.close()
                if f_private != None:
                    f_private.close()
        else:
            self.tKey.insert(INSERT, 'There is no Keys to save')
        #print(self.Keys.private_key.to_string())

    def import_keys_from_file(self):
        #filename = fd.askopenfilename()
        #old_keys = self.Keys
        filetype1 = (
            ('public key files', '*.public'),
            ('All files', '*.*')
        )
        filetype2 = (
            ('private key files', '*.private'),
            ('All files', '*.*')
        )
        f_public = None
        f_private = None
        try:
            filename_public = fd.askopenfilename(
                title='Save public key file',
                initialdir='.',
                filetypes=filetype1,
                defaultextension='.public')
            f_public = open(filename_public, 'rb')

            filename_private = fd.askopenfilename(
                title='Save private key file',
                initialdir='.',
                filetypes=filetype2,
                defaultextension='.private')
            f_private = open(filename_private, 'rb')

            # f_public = open('key.public', 'rb')
            # f_private = open('key.private', 'rb')
            self.Keys = KeyRing()

            public_key_poly_size = int.from_bytes(f_public.read(2), 'big')
            f_public.read(1)
            pub_key_str = f_public.read(public_key_poly_size)
            self.Keys.public_key.init_str(pub_key_str,public_key_poly_size)
            a_str = f_public.read(public_key_poly_size)
            self.init_polynomial_a.init_str(a_str, public_key_poly_size)

            private_key_poly_size = int.from_bytes(f_private.read(2), 'big')
            f_private.read(1)
            prive_key_str = f_private.read()
            #print(prive_key_str)
            self.Keys.private_key.init_str(prive_key_str, private_key_poly_size)

            key_str = self.Keys.print_bytes()#.decode('latin-1')
            self.tKey.delete('1.0', END)
            #self.tKey['state'] = 'enabled'
            self.tKey.insert(INSERT, "Initial polynomial a:\n")
            self.tKey.insert(INSERT, self.init_polynomial_a.print_bytes() + b'\n')
            self.tKey.insert(INSERT, key_str)
        finally:
            if f_public != None:
                f_public.close()
            if f_private != None:
                f_private.close()

        # if old_keys.public_key == self.Keys.public_key:
        #     self.tKey.insert(INSERT, 'public key = OK\n')
        # else:
        #     self.tKey.insert(INSERT, 'public key = NOK\n')
        #
        # if old_keys.private_key == self.Keys.private_key:
        #     self.tKey.insert(INSERT, 'private key = OK\n')
        # else:
        #     self.tKey.insert(INSERT, 'private key = NOK\n')

    def _encrypt_str(self, public_key, a_init, str_to_enc):
        m = BinPolynomial()
        m.init(0, a_init.get_poly_mod())
        block_size = a_init.get_poly_mod() // 8
        block_num = len(str_to_enc) // block_size
        q = len(str_to_enc) % block_size
        if q != 0:
            block_num += 1
        vec = list()
        i = 0
        for j in range(block_num):
            m.init_str(str_to_enc[i:i+block_size])
            vec.append(self.encryptor.encrypt(public_key, m, a_init))
            i += block_size
        return vec

    def _decrypt_str(self, private_key, cipher_text_list):
        res = bytes()
        m = BinPolynomial()
        m.init(0, private_key.get_poly_mod())
        for cipher_obj in cipher_text_list:
            m = self.encryptor.decrypt(private_key, cipher_obj)
            res += m.to_bytes()
        return res

    def encrypt_txt(self):
        input_str = self.input_text.get()
        self.EncryptOutputText.delete('1.0', END)
        try:
            if self.Keys != None:
                self.encrypted_str = self._encrypt_str(self.Keys.public_key,
                                                    self.init_polynomial_a,
                                                    input_str)

                for cipher_obj in self.encrypted_str:
                    output_str = cipher_obj.print_bytes()
                    self.EncryptOutputText.insert(INSERT, output_str)
            else:
                self.EncryptOutputText.insert(INSERT, "Error: No encryption Keys selected")
        except:
            self.EncryptOutputText.insert(INSERT, "Error: Unexpected error")

    def decrypt_txt(self):
        try:
            #input_str = self.input_text.get()
            self.DecryptOutputText.delete('1.0', END)
            if self.Keys != None and self.encrypted_str != None:
                self.decrypted_str = self._decrypt_str(self.Keys.private_key,
                                                       self.encrypted_str)
                self.DecryptOutputText.insert(INSERT, self.decrypted_str.decode('latin1'))
            elif self.Keys == None :
                self.DecryptOutputText.insert(INSERT, "Error: No encryption Keys selected")
            elif self.encrypted_str == None:
                self.DecryptOutputText.insert(INSERT, "Error: No encrypted text")
        except:
            self.DecryptOutputText.insert(INSERT, "Error: Unexpected error")

    def encrypt_file(self):
        self.EncryptOutputText.delete('1.0', END)
        filetype = (
            ('All files', '*.*')
        )
        filename = fd.askopenfilename(
            title='Open file',
            initialdir='.',
            #filetypes=filetype
        )
        f = None
        f_enc = None
        try:
            try:
                file_size = os.path.getsize(filename)
                f = open(filename, 'rb')
                file_str = f.read(file_size)
                f_enc = open(filename+'.blwe', 'wb')
                if self.Keys != None:
                    self.encrypted_str = self._encrypt_str(self.Keys.public_key,
                                                       self.init_polynomial_a,
                                                       file_str)

                    global global_var
                    global_var = self.encrypted_str

                    public_poly_size = self.Keys.public_key.get_poly_mod()

                    self.EncryptOutputText.insert(INSERT, 'Encrypted text is saved in file: ' + filename +'.blwe \n\n')

                    f_enc.write(public_poly_size.to_bytes(2, 'big'))
                    f_enc.write(b'\n')
                    output_str = bytes()
                    bytes_str = bytes()
                    for cipher_obj in self.encrypted_str:
                        output_str += cipher_obj.print_bytes()
                        bytes_str += cipher_obj.to_bytes()

                    self.EncryptOutputText.insert(INSERT, output_str.decode('latin1'))
                    f_enc.write(bytes_str)

                else:
                    self.EncryptOutputText.insert(INSERT, "Error: No encryption Keys selected")
            except:
                self.EncryptOutputText.insert(INSERT, "Error: Unexpected error")
        finally:
            if f != None:
                f.close()
            if f_enc != None:
                f_enc.close()

    def read_encrypted_file(self):
        filetype = (
            ('Encrypted', '*.blwe'),
            ('All files', '*.*')
        )
        filename = fd.askopenfilename(
            title='Open file',
            initialdir='.',
            filetypes=filetype
        )
        f_enc = None
        vec = None
        expected_key_size = 0
        try:

            file_size = os.path.getsize(filename)
            f_enc = open(filename, 'rb')
            file_str = f_enc.read(file_size)
            expected_key_size = int.from_bytes(file_str[:2], 'big')
            block_size = (expected_key_size)
            block_num = (file_size-3) // block_size
            vec = list()
            i = 3
            for j in range(block_num//2):
                m = CipherText()
                m.c1.init_str(file_str[i:i + block_size],expected_key_size)
                i += block_size
                m.c2.init_str(file_str[i:i + block_size],expected_key_size)
                i += block_size
                vec.append(m)
        finally:
            if f_enc != None:
                f_enc.close()
        return expected_key_size, filename, vec

    def decrypt_file(self):
        self.DecryptOutputText.delete('1.0', END)
        f = None
        try:
            try:
                expected_key_size, filename, self.encrypted_str = self.read_encrypted_file()
                f = open(filename[:-5], 'wb')
                global global_var
                for i in range(len(global_var)):
                    if global_var[i] == self.encrypted_str[i]:
                        print("OK")

                if self.Keys != None and self.Keys.private_key.get_poly_mod() == expected_key_size:
                    self.decrypted_str = self._decrypt_str(self.Keys.private_key,
                                                            self.encrypted_str)

                    self.DecryptOutputText.insert(INSERT, 'Encrypted text is saved in file: ' + filename[:-5])
                    f.write(self.decrypted_str)

                else:
                    self.DecryptOutputText.insert(INSERT, "Error: No encryption Keys selected")
            except:
                self.DecryptOutputText.insert(INSERT, "Error: Unexpected error")
        finally:
            if f != None:
                f.close()

if __name__ == "__main__":
    app_win = MainAppWindow()

















