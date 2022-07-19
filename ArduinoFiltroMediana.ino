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

int iniciaRecebCab = 0;
int flagSerial = 0;
int contSerial = 0;
byte comandoSerial = 0;
byte incomingByte = 0;
unsigned int linhasSerial = 0;
unsigned int colunasSerial = 0;
int received_number = 0;
String received_string = "";
char received_char = 0;
int a = 0; //variavel p execução do while uma unica vez
long int qtdElementos1;
unsigned int height = 1; //variavel para aplicacao na funcao mediana correspondente às linhas
unsigned int width = 1;  //variavel para aplicacao na funcao mediana correspondente às colunas
int comandoEnvioPython = 0;

void aplicaMediana(
    int height,         // input - especifica quantidade de linhas da matriz destino
    int width,          // input - especifica quantidade de colunas da matriz destino
    uint8_t Mat[3][200] // input - matriz origem, correspondente aos dados colhidos do arquivo //
)
{
  int pixelEmAnalise = width + 1; //MODIFICADO -> será analisado apenas a linha do meio do lote
  int linhas = height - 1;
  int colunas = width - 1;
  int vetMediana[9];

  unsigned int controleElementosMatriz = (height) * (width);
  int indiceMediana, controleAtualizado, linhaSelecionada, colunaSelecionada, controle, col, lin;
  int temp = 0;
  int Mediana = pixelEmAnalise;
  int contador = 0;
  int i, j;

  // "varre" / consulta todas as posições da Matriz recebida do arquivo origem
  while (pixelEmAnalise <= 2 * width) //MODIFICADO -> será analisado apenas a linha do meio do lote, no caso o controleElementosMatriz será o valor de 2*qtd de colunas
  {
    i = pixelEmAnalise / (colunas + 1);
    j = pixelEmAnalise % (colunas + 1);

    // se j for igual a zero significa que o elemento em analise pertence a ultima posicao da linha i anterior a linha em analise
    if (j == 0)
    {
      i = i - 1;
      j = colunas;
    }
    // caso contrario o elemento faz parte da linha i atual, porem j tem de subtraido 1 jah q a posicao zero das colunas tem de ser contada
    else
    {
      j--;
    }

    // linhaSelecionada e colunaSelecionada armazenam indices de linha e coluna (respectivamente) do elemento em analise
    linhaSelecionada = i;
    colunaSelecionada = j;

    // variavel lin e col armazenam os indices de linha e coluna do primeiro elemento a compor a mascara do filtro
    // variavel controle servira para controlar posicao dos elementos a serem salvos no vetor da mascara a ser ordenado
    lin = i - 1;
    col = j - 1;
    controle = 0;

    // Verifica se é algum pixel da borda da imagem e complementa a mascara com zeros
    // para as posicoes inexistentes - zero padding
    if (i == 0 || j == 0 || i == linhas || j == colunas)
    {
      for (i = lin; i <= lin + 2; i++)
      {
        for (j = col; j <= col + 2; j++)
        {
          // zero padding para as posicoes de elementos que facam parte das bordas externas da imagem
          if (i < 0 || j < 0 || i > linhas || j > colunas)
          {
            vetMediana[controle] = 0;
          }
          // caso exista elemento vizinho ao pixel em analise este sera armazenado no vetorMediana
          else
          {
            //pgm_read_byte(&(imagem[i][j])) - chama funcao de leitura do valor armazenado no endereço correpsondente a imagem[i][j]
            vetMediana[controle] = Mat[i][j];
          }
          controle++;
        }
      }
    }
    // elementos que nao fazem parte das bordas externas sao armazenado no vetorMediana
    // sem necessidade de complemnto de valores zero aos elementos faltantes da mascara a ser armazenada no vetor
    else
    {
      for (i = lin; i <= lin + 2; i++)
      {
        for (j = col; j <= col + 2; j++)
        {
          vetMediana[controle] = Mat[i][j];
          controle++;
        }
      }
    }
    controle--;

    // ordena vetor com a mascara dos vizinhos do pixel selecionado
    temp = 0;
    controleAtualizado = controle;
    i = 0;
    for (i = 0; i <= controle; i++)
    {
      for (j = 0; j <= controleAtualizado; j++)
      {
        if (vetMediana[j] > vetMediana[controleAtualizado])
        {
          temp = vetMediana[j];
          vetMediana[j] = vetMediana[controleAtualizado];
          vetMediana[controleAtualizado] = temp;
        }
      }
      controleAtualizado--;
    }

    // valor da mediana do vetor que substituira o pixel selecionado
    indiceMediana = controle / 2;

    if ((controle % 2) == 0)
    {
      Mediana = vetMediana[indiceMediana];
    }
    else
    {
      Mediana = (vetMediana[indiceMediana] + vetMediana[indiceMediana + 1]) / 2;
    }

    // Impressao da Matriz Mediana direto na Serial
    if (((colunaSelecionada + 1) % (width) == 0))
    {
      //insere comandos para identificar ao python o envio da matriz resposta
      comandoEnvioPython++;
      Serial.print(Mediana);
      Serial.print(F("\n"));
    }
    else
    {
      Serial.print(Mediana);
      Serial.print(F("\t"));
    }
    pixelEmAnalise++;
  }
}

// ###############################################################################################################
int iniciaValoresCab = 0;

void setup()
{
  // Inicia a comunicação serial
  Serial.begin(115200);

  delay(1000);
  //AVISO DE SERIAL CONCLUIDA
  Serial.println("I");
}

int contadorCaracteres = 0;
int valorComando = 256;
int iniciaReceb = 0;
int flagCabecalho = 0;

int contador = 0;
int flagLinhasOk = 0;                   //flag criada para sanar o erro na leitura dos valores respectivos ao tamanho original da matriz (cabecalho)
int limiteColunas = 6;                  //fator limitante para largura da matriz a ser recebida (qntd de colunas a serem tratadas por lote de analise)
int contadorRecebimentoLotesMatriz = 0; //variavel para termos controle de quantos lotes já foram recebidos da matriz original
uint8_t matrizProvisoria[3][200];       //############<<<<<<<<<<<<<<<@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int contaQtdValoresMatriz = 0;          //variavel para controle de qtd de valores a serem aguardadas na serial por lote
int qtdElementosPorLote = 0;
int contaQtdValoresMatrizGeral = 0;
int chamadaLeituraComando = 0;

int comandoInstrucao()
{
  while (Serial.available() > 0 && iniciaRecebCab != 1)
  {
    received_string += Serial.readString();
    iniciaRecebCab = 2;
    if (iniciaReceb == 1 && iniciaValoresCab == 1)
      contSerial++;
    if (iniciaReceb == 2 && flagSerial == 1)
    {
      contaQtdValoresMatriz++;
      contaQtdValoresMatrizGeral++;
    }
    contadorCaracteres++;
  }
  received_number = received_string.toInt();
  received_string = "";
  contadorCaracteres = 0;
  chamadaLeituraComando++;
  return received_number;
}

int statusMandarRestoLote = 0;
int qtdTotalDeLotes = 0;
int contadorTotalDeLotes = 0;

void loop()
{

  //while apenas para executar a funcao mediana uma unica vez
  while (a != 1)
  {
    //-- LÊ CABEÇALHO E IDENTIFICA TRANSIÇÃO DE VALORES - rotinas iniciam ao receber valor "-1" na serial
    while (iniciaReceb == 0)
    {

      valorComando = comandoInstrucao();

      //se valorComando == -1 e flagSerial == 0, significa q estou a iniciar recebimento do cabecalho
      if (valorComando == 1 && flagSerial == 0)
      { //MODIFICADO AQUI 23/06 - 20:24 VALORCOMANDO = -1
        //se iniciaReceb == 1 - significa que iremos receber os valores de cabecalho (qtd de linhas e ecolunas da imagem a ser filtrada)
        //se iniciaRecebCab == 1 - significa que iremos finalizar o recebimento dos valores de cabecalho (qtd de linhas e ecolunas da imagem a ser filtrada)
        //se iniciaReceb == 2 - significa que iremos iniciar o recebimento dos valores das posições da matriz imagem (valores dos pixels da imagem a ser filtrada)
        iniciaReceb = 1;
        Serial.println(valorComando);
      }

      //se valorComando == -1 e flagSerial == 1, significa q estou a inicar recebimento de valores correspondentes a matriz original
      if (valorComando == 1 && flagSerial == 1)
      {
        iniciaReceb = 2;
        Serial.println("mande lote1");
      }
      valorComando = 256;
    }

    //###########>>>>>>>>>>>> tratamento dos comandos recebidos após inicio ou termino de comunicacao

    //recebe valores respectivos de linhas e colunas da matriz da imagem a ser filtrada
    while (iniciaReceb == 1)
    {
      iniciaValoresCab = 1;
      valorComando = comandoInstrucao();
      if (valorComando != 256 && flagSerial == 0)
      {
        if (flagCabecalho == 0)
        {
          if (contSerial == 1 && flagLinhasOk != 1)
          {
            linhasSerial = valorComando;
            flagLinhasOk = 1;
            Serial.println(linhasSerial);
          }
          if (contSerial == 2)
          {
            colunasSerial = valorComando;
            Serial.println(colunasSerial);
            flagSerial = 1;
            iniciaReceb = 0;
            qtdElementos1 = linhasSerial * colunasSerial;
            qtdElementosPorLote = 3 * colunasSerial;
            qtdTotalDeLotes = linhasSerial;
          }
        }
      }
      valorComando = 256;
    }
    int linhaInterna, colunaInterna;

    //atribui valores recebidos pela serial à matriz provisoria para aplicacao do filtro por partes
    while (iniciaReceb == 2 && flagSerial == 1 && contadorTotalDeLotes <= linhasSerial)
    {

      while (contadorTotalDeLotes <= linhasSerial && Serial.available() > 0)
      {
        while (contaQtdValoresMatriz < qtdElementosPorLote && Serial.available() > 0 && statusMandarRestoLote == 0)
        {
          linhaInterna = (contaQtdValoresMatriz + 1) / (colunasSerial);
          colunaInterna = (contaQtdValoresMatriz + 1) % (colunasSerial);

          // se j for igual a zero significa que o elemento em analise pertence a ultima posicao da linha i anterior a linha em analise
          if (colunaInterna == 0)
          {
            linhaInterna = linhaInterna - 1;
            colunaInterna = (colunasSerial - 1); //CONFIRMAR ISTO AQUI
          }
          // caso contrario o elemento faz parte da linha i atual, porem j tem de subtraido 1 jah q a posicao zero das colunas tem de ser contada
          else
          {
            colunaInterna--;
          }

          //ATENTE-SE PARA POSSIVEIS ERROS NA HORA DE RECEBER O VALOR DO COMANDO/PIXEL, POIS PODE OCORRE SOBRESCRIÇÃO (COMO NO ENTENDIMENTO DA LEITURA DO CABECALHO)
          valorComando = comandoInstrucao();
          matrizProvisoria[linhaInterna][colunaInterna] = valorComando;
          Serial.println(valorComando);

        if (linhaInterna == 2 && colunaInterna == (colunasSerial - 1) && received_string == "")
        {
          contadorTotalDeLotes++;

          Serial.println(F("w"));
          aplicaMediana(3, colunasSerial, matrizProvisoria);
          Serial.println(F("w"));
          statusMandarRestoLote = 1;
        }
        }
        if (statusMandarRestoLote == 1)
        {
          statusMandarRestoLote = 0;
          contaQtdValoresMatriz = 0;
        }
      }
      valorComando = 256;

      if (contadorTotalDeLotes == linhasSerial)
      {
        Serial.println(F("\n\n\n"));
        Serial.println(F(">> PROGRAMA FINALIZADO <<"));
        contadorTotalDeLotes++;
        a = 1;
      }
    }
  }

  //APLICA FILTRO
  if (a == 1 && contador == 0)
  {
    contador++;

    Serial.println(F("\n\n\n"));
    Serial.print(F("linhasSerial = "));
    Serial.println(linhasSerial);

    Serial.print(F("colunasSerial = "));
    Serial.println(colunasSerial);
  }
}
