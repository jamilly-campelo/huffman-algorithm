# EDB2---Projeto-da-1-unidade-compress-o-de-dados.
Esse programa vai adotar uma tabela de frequência para gerar uma codificação base. A codificação dos símbolos pode ser hard-coded (ou seja, você vai adotar como padrão para não ser necessário guardar a árvore de Huffman no arquivo comprimido). Essa codificação será usada para comprimir ou descomprimir arquivos de texto em geral


para compilar basta

    make


    ./bin/compressor src/Compressor/Tabela_Base.txt src/Compressor/teste.cpp outputs/teste.huf
    
    or 

    ./bin/compressor src/Compressor/teste.cpp outputs/teste.huf


para testar se deu certo

    # Verificar se o arquivo foi criado
    ls -la outputs/teste.huf

    # Comparar tamanhos
    wc -c src/Compressor/teste.cpp outputs/teste.huf

    # Ver conteúdo em hexadecimal
    hexdump -C outputs/teste.huf | head -5



        huffman-algorithm/
    ├── src/
    │   └── Compressor/
    │       ├── compressor.cpp      # Implementação do compressor
    │       ├── compressor.hpp      # Interface do compressor
    │       ├── huffman_tree.cpp    # Implementação da árvore de Huffman
    │       ├── huffman_tree.hpp    # Interface da árvore de Huffman
    │       ├── file_reader.hpp     # Utilitários de leitura de arquivo
    │       ├── main.cpp            # Programa principal
    │       ├── Tabela_Base.txt     # Tabela de frequências padrão
    │       └── teste.cpp           # Arquivo de teste
    ├── outputs/                    # Arquivos comprimidos gerados
    ├── obj/                       # Arquivos objeto (gerados)
    ├── bin/                       # Executável (gerado)
    └── makefile                   # Script de compilação


# Exemplo de uso 

    # Compilar o projeto
    make

    # Comprimir um arquivo de código-fonte
    ./bin/compressor src/Compressor/Tabela_Base.txt src/Compressor/teste.cpp outputs/teste.huf

    # Verificar taxa de compressão
    echo "Taxa de compressão:"
    echo "scale=2; $(wc -c < outputs/teste.huf) / $(wc -c < src/Compressor/teste.cpp) * 100" | bc -l
    echo "% do tamanho original"


## comando uteis

    # Recompilar tudo
    make rebuild

    # Limpar arquivos gerados
    make clean

    # Ajuda
    make help



## Requisitos

    Compilador C++17 (g++ 7.0 ou superior)

    Make

    Sistema Linux/Unix