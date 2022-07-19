# /* IFCE - Engenharia de Computacao
#  * Disciplina: Sistemas Embarcados
#  * Professor: Elias Teodoro
#  * Trabalho 3 - Etapa 2  - 06/2022
#  * Autor1: Gabriel Oliveira Campelo
#  * Autor2: Rian Araujo dos Santos
#  * Função: Programa lê uma imagem PGM P2, aplica filtro mediana e exporta imagem PGM P2
#  * Plataforma alvo: Arduino UNO com Sistema Operacional Linux
#  *
#  * ref1 - repositorio de imagens pgm p2, com ruidos
#  * https://www.ime.usp.br/~reverbel/mac110-BCC-07/eps/ep4-imagens-sujas/
#  *
#  * ref2.0 e ref2.1 (respectivamente) - interpretacao do filtro mediana
#  * https://www.researchgate.net/publication/281749509_Uso_de_Filtros_em_Imagens_Digitais_de_Sementes_de_Milho
#  * https://www.youtube.com/watch?v=eJx3g-ZEfm4
#  *
#  * ref3 - modelo de importacao, leitura e exportacao de imagem p2
#  * https://fractalcoding.blogspot.com/2019/10/image-processing-in-c.html
#  *
#  * Este programa foi escrito para atender as requisicoes do Trabalho 3
#  * da segunda etapa da disciplina de Sistemas Embarcados do curso de
#  * Engenharia de Computacao do Instituto Federal do Ceara que sao:
#  *
#  * OBSERVACOES IMPORTANTES:
#  *
#  * 1- Os arquivos PGM P2 devem ter no máximo o valor de 255 (8 bits escala de cinza) atribuido a cada pixel
#  * 2- Imagens PGM P2 sem comentarios em sua estrutura (2a linha do arquivo) nao funcionarao
#  * 3- Imagens exportadas (PGM P2) incluem comentarios em sua estrutura
#  * 4- Imagens PGM P5 nao funcinarao com este programa
#  * 5- Como a parte de leitura da imagem e gravacao foram baseadas na ref3 (ver acima)
#  *    temos variaveis que chamam-se width (correspondentes a colunas - columns) e
#  *    variaveis height (correspondentes a linhas - rows)
#  *
#  *
#  * INPUTs:
#  *  - Programa solicita que seja inserido o nome da imagem a passar pelo filtro da mediana
#  *  - Digitar o nome da imagem incluindo a extensao (exemplo: "imagemOriginal.pgm")
#  *  - Programa solicita que seja inserido o nome da imagem a ser salva
#  *  - Digitar o nome da imagem incluindo a extensao (exemplo: "imagemTratada.pgm")
#  *
#  * OUTPUT:
#  *  - Imagem gerada apos passagem pelo filtro mediana no Arduino
#  *
#  */

import serial
import numpy as np
import time

# encontra portas seriais ativas
ports = []
for n in range(50):
    try:
        ports.append(serial.Serial("COM" + str(n)).port)
    except:
        pass

totalColunas = 0
flagAlternaComunicacao = 0
arduino = ""
contaInstrucoes = 0
i = 0
value = 0

dicio = {0: b'0', 1: b'1', 2: b'2', 3: b'3', 4: b'4', 5: b'5', 6: b'6', 7: b'7', 8: b'8', 9: b'9', 10: b'10', 11: b'11', 12: b'12', 13: b'13', 14: b'14',
         15: b'15',	16: b'16',	17: b'17',	18: b'18',	19: b'19',	20: b'20',	21: b'21',	22: b'22',	23: b'23',	24: b'24',	25: b'25',	26: b'26',
         27: b'27',	28: b'28',	29: b'29',	30: b'30',	31: b'31',	32: b'32',	33: b'33',	34: b'34',	35: b'35',	36: b'36',	37: b'37',	38: b'38',
         39: b'39',	40: b'40',	41: b'41',	42: b'42',	43: b'43',	44: b'44',	45: b'45',	46: b'46',	47: b'47',	48: b'48',	49: b'49',	50: b'50',
         51: b'51',	52: b'52',	53: b'53',	54: b'54',	55: b'55',	56: b'56',	57: b'57',	58: b'58',	59: b'59',	60: b'60',	61: b'61',	62: b'62',
         63: b'63',	64: b'64',	65: b'65',	66: b'66',	67: b'67',	68: b'68',	69: b'69',	70: b'70',	71: b'71',	72: b'72',	73: b'73',	74: b'74',
         75: b'75',	76: b'76',	77: b'77',	78: b'78',	79: b'79',	80: b'80',	81: b'81',	82: b'82',	83: b'83',	84: b'84',	85: b'85',	86: b'86',
         87: b'87',	88: b'88',	89: b'89',	90: b'90',	91: b'91',	92: b'92',	93: b'93',	94: b'94',	95: b'95',	96: b'96',	97: b'97',	98: b'98',
         99: b'99',	100: b'100', 101: b'101', 102: b'102',	103: b'103', 104: b'104', 105: b'105',	106: b'106', 107: b'107',
         108: b'108', 109: b'109', 110: b'110', 111: b'111', 112: b'112', 113: b'113', 114: b'114',	115: b'115', 116: b'116',
         117: b'117', 118: b'118', 119: b'119', 120: b'120', 121: b'121', 122: b'122', 123: b'123',	124: b'124', 125: b'125',
         126: b'126', 127: b'127', 128: b'128', 129: b'129', 130: b'130', 131: b'131', 132: b'132',	133: b'133', 134: b'134',
         135: b'135', 136: b'136', 137: b'137', 138: b'138', 139: b'139', 140: b'140', 141: b'141',	142: b'142', 143: b'143',
         144: b'144', 145: b'145', 146: b'146', 147: b'147', 148: b'148', 149: b'149', 150: b'150',	151: b'151', 152: b'152',
         153: b'153', 154: b'154', 155: b'155', 156: b'156', 157: b'157', 158: b'158', 159: b'159',	160: b'160', 161: b'161',
         162: b'162', 163: b'163', 164: b'164', 165: b'165', 166: b'166', 167: b'167', 168: b'168',	169: b'169', 170: b'170',
         171: b'171', 172: b'172', 173: b'173',	174: b'174', 175: b'175', 176: b'176', 177: b'177',	178: b'178', 179: b'179',
         180: b'180', 181: b'181', 182: b'182',	183: b'183', 184: b'184', 185: b'185', 186: b'186',	187: b'187', 188: b'188',
         189: b'189', 190: b'190', 191: b'191',	192: b'192', 193: b'193', 194: b'194', 195: b'195',	196: b'196', 197: b'197',
         198: b'198', 199: b'199', 200: b'200',	201: b'201', 202: b'202', 203: b'203', 204: b'204',	205: b'205', 206: b'206',
         207: b'207', 208: b'208', 209: b'209',	210: b'210', 211: b'211', 212: b'212', 213: b'213',	214: b'214', 215: b'215',
         216: b'216', 217: b'217', 218: b'218',	219: b'219', 220: b'220', 221: b'221', 222: b'222',	223: b'223', 224: b'224',
         225: b'225', 226: b'226', 227: b'227',	228: b'228', 229: b'229', 230: b'230', 231: b'231',	232: b'232', 233: b'233',
         234: b'234', 235: b'235', 236: b'236',	237: b'237', 238: b'238', 239: b'239', 240: b'240',	241: b'241', 242: b'242',
         243: b'243', 244: b'244', 245: b'245',	246: b'246', 247: b'247', 248: b'248', 249: b'249', 250: b'250', 251: b'251',
         252: b'252', 253: b'253', 254: b'254', 255: b'255'}

vetorConverte = np.arange(0, 256, dtype=np.uint8)

# se nenhuma porta ativa avisa q o arduino n esta conectado
if len(ports) == 0:
    input("\nArduino não conectado.")
    quit()
elif len(ports) == 1:
    port_com = ports[0]
else:
    print("\nPortas disponíveis:")
    for n in ports:
        print("\t" + n)
    port_com = "COM" + input("\nInforme o número da porta: ")

imagem = input("Digite o nome da imagem a ser lida (incluindo .pgm): ")
arquivo = open(imagem)

texto = arquivo.readlines()
cabecalho = texto[2:3]
corte = str(cabecalho).strip('[]').strip("'").strip("\\n").split()
totalColunas = int(corte[0])
totalLinhas = int(corte[1])
totalColunasBytes = dicio[vetorConverte[totalColunas]]
totalLinhasBytes = dicio[vetorConverte[totalLinhas]]
print("totalColunas = ", totalColunas)
print("totalLinhas = ", totalLinhas)
texto2 = texto[4:]
tam = len(texto2)
saidaArduino = []
vetorBorda = np.zeros(totalColunas, dtype=np.uint8)
print("vetorBorda = ", vetorBorda)

for i in range(0, tam):
    texto2[i] = texto2[i].replace('\t', ' ')
    texto2[i] = texto2[i].replace('\n', '')

controleLaco = 1
comunicacaoAlternancia = 0
contadorInteracoes = 0
numeroLote = 0
incremento = 0
contaElementosEnviadosParaMatrizArduino = 0
posicoesSaida = 0
flagNP = 0
tamanhoVetorEnviar = 0

try:
    port = serial.Serial(port_com, 115200, timeout=0.5)
    print("\nPorta conectada: ", port.portstr)

    # inicia comunicacao com a serial

    while port.isOpen() and controleLaco != 7:
        arduino = str(port.readline().decode())
        while (controleLaco != 0):
            arduino = str(port.readline().decode())
            if arduino != "" and comunicacaoAlternancia == 0:
                print(contadorInteracoes)
                comunicacaoAlternancia = 1
                print("Resposta Arduino -> " + arduino)
                contadorInteracoes = contadorInteracoes + 1

            if contadorInteracoes == 1 and comunicacaoAlternancia == 1:
                comunicacaoAlternancia = 0
                print("# envia 1 para enviar cabecalho")
                port.write(b'1')

            if contadorInteracoes == 2 and comunicacaoAlternancia == 1:
                comunicacaoAlternancia = 0
                print("# envia qtd linhas")
                port.write(totalLinhasBytes)

            if contadorInteracoes == 3 and comunicacaoAlternancia == 1:
                comunicacaoAlternancia = 0
                print("# envia qtd colunas")
                port.write(totalColunasBytes)

            if contadorInteracoes == 4 and comunicacaoAlternancia == 1:
                comunicacaoAlternancia = 0
                print("# envia 1 para enviar lotes da imagem")
                port.write(b'1')
            if (contadorInteracoes == 5 and comunicacaoAlternancia == 1):
                controleLaco = 0
        print("--------------------------------------------------")
        while (controleLaco == 0):
            while (numeroLote < totalLinhas):
                print("--------------------------------------------------")
                print("numeroLote = ", numeroLote)
                if numeroLote == 0:
                    numeroLoteProvisorio = numeroLote
                else:
                    numeroLoteProvisorio = numeroLote - 1

                for i in range(3):
                    if (((i == 0) and (numeroLote == 0)) or ((i == 2) and (numeroLote == totalLinhas-1))):
                        # envia linhaDeBorda como primeira linha
                        vetorEnviar = vetorBorda
                        tamanhoVetorEnviar = len(vetorEnviar)
                        flagNP = 1
                    else:
                        if (numeroLote == 0 and i == 2):
                            vetorEnviar = texto2[numeroLote + 1].split(' ')
                        else:
                            vetorEnviar = texto2[numeroLoteProvisorio].split(' ')
                            numeroLoteProvisorio = numeroLoteProvisorio + 1
                        vetorEnviar = [int(i) for i in vetorEnviar]
                        flagNP = 0


                    for j in range(0, len(vetorEnviar)):
                        if vetorEnviar[j] != " " and vetorEnviar[j] != "\n":
                            value = vetorEnviar[j]
                            if (flagNP == 1):
                                valor = dicio[vetorEnviar[j]]
                                port.write(valor)
                            else:
                                port.write(dicio[vetorConverte[value]])
                            time.sleep(0.3)
                            flagAlternaComunicacao = 0
                            while (flagAlternaComunicacao == 0):
                                arduino = str(port.readline().decode())
                                if arduino != "" and flagAlternaComunicacao == 0:
                                    flagAlternaComunicacao = 1
                            contaElementosEnviadosParaMatrizArduino = contaElementosEnviadosParaMatrizArduino + 1

                contadorX = 0

                while (contaElementosEnviadosParaMatrizArduino == totalColunas*3):
                    if port.inWaiting() > 0 and contaElementosEnviadosParaMatrizArduino != 0:
                        arduino = str(port.readline().decode())
                        contadorX = contadorX + 1
                        if (contadorX == 2):
                            saidaArduino.append(arduino)
                            print("saidaArduino -> ", saidaArduino)
                        elif (contadorX == 3):
                            contaElementosEnviadosParaMatrizArduino = 0

                posicoesSaida = posicoesSaida + 1
                print(">>>>>>> lote enviado = ", numeroLote)
                numeroLote = numeroLote + 1
            print(">>>>>>> FIM DO PROCESSAMENTO DO FILTRO NA IMAGEM <<<<<<<")
            controleLaco = 7

    imagem2 = input("Digite o nome da imagem a ser gravada (incluindo .pgm): ")

    with open(imagem2, "w") as arquivo2:
        arquivo2.write("%s" % texto[0])
        arquivo2.write("%s" % texto[1])
        arquivo2.write("%s" % texto[2])
        arquivo2.write("%s" % texto[3])
        for item in saidaArduino:
            arquivo2.write("%s" % item)

    print("\nTAREFA CONCLUIDA\n")

except Exception as ex:
    input("\nERRO: " + str(ex))
