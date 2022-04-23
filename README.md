# Indexer

Link para arquivos de testes: http://200.236.3.126:8999/ds143-texts/

## Build and run
Para buildar o projeto, basta usar um compilador de linguagem C, como, por exemplo, gcc:
```
$ gcc indexer.c -o indexer
```

Então rodar o programa:
```
$ ./indexer --freq N ARQUIVO
$ ./indexer --freq-word PALAVRA ARQUIVO
$ ./indexer --search TERMO ARQUIVO [ARQUIVO ...]
```

## Descrição
O programa **indexer** realiza uma contagem de palavras em documentos de texto. A partir dessa contagem, ele ainda permite uma busca pelo número de ocorrências de uma palavra específica em um documento, ou a seleção de documentos relevantes para um dado termo de busca.O programa **indexer** transforma todas as letras para minúsculas e ignora caracteres como números e pontuações. Quando executado com a opção --freq, o programa **indexer** irá exibir o número de ocorrências das N palavras mais frequentes no documento passado como parâmetro, em ordem decrescente de ocorrências. Quando executado com a opção --freq-word, o programa **indexer** exibe a contagem de uma palavra específica no documento passado como parâmetro. Por fim, quando executado com a opção --search, o programa **indexer** apresenta uma listagem dos documentos mais relevantes para um dado termo de busca. Para tanto, o programa utiliza o cálculo TF-IDF (Term Frequency-Inverse Document Frequency).

## Opções
|Opção|Parâmetro inicial|Parâmetros adicionais|Descrição|
|-----|-----|-----|-----|
|*--freq*|N (*int*)|ARQUIVO (*string*)|Exibe o número de ocorrência das N palavras que mais aparecem em ARQUIVO, em ordem decrescente de ocorrência.|
|*--freq-word*|PALAVRA (*string*)|ARQUIVO (*string*)|Exibe o número de ocorrências de PALAVRA em ARQUIVO. |
|*--search*|TERMO (*string*)|ARQUIVO1 (*string*) ARQUIVO2 (*string*) ...|Exibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por TERMO. A listagem é apresentada em ordem descrescente de relevância. TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre àspas.|