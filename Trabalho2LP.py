import pickle

class contato:
    def __init__(self, nome, telefone, endereco, relacao):
        self.__nome = nome
        self.__telefone = telefone
        self.__endereco = endereco
        self.__relacao = relacao

    def get_nome(self):
        return self.__nome

    def get_telefone(self):
        return self.__telefone

    def get_endereco(self):
        return self.__endereco

    def get_relacao(self):
        return self.__relacao

    def set_telefone(self, telefone):
        self.__telefone = telefone

    def set_endereco(self, endereco):
        self.__endereco = endereco

    def set_relacao(self, relacao):
        self.__relacao = relacao

    def get_nome_telefone_endereco_relacao(self):
        print(f"Contato encontrado: {self.__nome}, {self.__telefone}, {self.__endereco}, {self.__relacao}.\n")

    def __str__(self): #toString p python, passando um obj da lista no print() n precisa chamar .toString()
        return f"Conteúdo do contato: {self.__nome}, Telefone: {self.__telefone}, Endereço - {self.__endereco}, Relação - {self.__relacao}"


class agenda:
    def __init__(self):
        self.__lista_contatos = []

    def altera_contato(self, novo_contato):
        contato_existente = self.busca_nome(novo_contato.get_nome())
        if contato_existente:
            contato_existente.set_telefone(novo_contato.get_telefone())
            contato_existente.set_endereco(novo_contato.get_endereco())
            contato_existente.set_relacao(novo_contato.get_relacao())

    def insere_contato(self, contato):
        contato_existente = self.busca_nome(contato.get_nome())
        if contato_existente:
            self.altera_contato(contato)
        else:
            self.__lista_contatos.append(contato)

    def remove_contato(self, nome):
        contato = self.busca_nome(nome)
        if contato:
            self.__lista_contatos.remove(contato)
            print(f"contato {nome} removido.\n")
        else:
            print(f"contato {nome} não encontrado.\n")

    def lista_contatos(self):
        for contato in self.__lista_contatos:
            print(contato)

    def busca_nome(self, nome):
        for contato in self.__lista_contatos:
            if contato.get_nome() == nome:
                return contato
        return None

    def salva_em_arquivo(self, nome_arquivo):
        with open(nome_arquivo, 'wb') as arquivo:
            pickle.dump(self.__lista_contatos, arquivo)
        print(f"\nAgenda salva em '{nome_arquivo}' com sucesso.")


class principal:
    def __init__(self):
        self.__agenda = agenda()

    def executa(self):
        contato1 = contato("Fulano", "99999999", "Rua A", "UFF")
        self.__agenda.insere_contato(contato1)

        contato2 = contato("Ciclano", "88888888", "Rua B", "CEDERJ")
        self.__agenda.insere_contato(contato2)

        contato3 = contato("Beltrano", "88889999", "Rua C", "Infância")
        self.__agenda.insere_contato(contato3)

        contato4 = contato("Fulano", "77777777", "Rua D", "UFF")
        self.__agenda.insere_contato(contato4)


        self.__agenda.remove_contato(contato2.get_nome())

        self.__agenda.lista_contatos()

        self.__agenda.salva_em_arquivo('contatos.pkl')

if __name__ == '__main__':
    app = principal()
    app.executa()
