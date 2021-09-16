//Desenvolvido por Alex Brosso e Willian Sumida

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct musica // Struct Música
{
    int id;
    char titulo[200];
    char artista[200];
    char album[200];
    int duracao; //segundos
} musica;

typedef struct NO* ArvAVL; // criar tipo arvore

struct NO{ //nos da arvore
    musica *info; //valores do proprio no
    int altura; // altura do no
    struct NO *esq; //aponta para no esq
    struct NO *dir; //aponta para no dir
};

typedef struct playlist_no //Struct Playlist
{
    musica *musica;
    struct playlist_no *prox;
} playlist_no;
playlist_no *iniPlaylist;

typedef struct lplaylists_no //Struct Lista de Playlists
{
    int id;
    char nome[200];
    playlist_no *musicas;
    struct lplaylists_no *prox;
} lplaylists_no;
lplaylists_no *iniPastaPlaylist;

ArvAVL* cria_ArvAVL(){ //criar a arvore
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

//FUNCAO MENU INICIAL
int menuInicio()
{
    system("cls");
    int opcaoInicio; //Variavel que armazena uma opção desejada pelo usuário.
    printf("\n----------MENU INICIAL----------\n");
    printf("\n1- Cadastrar musica");
    printf("\n2- Imprimir musicas");
    printf("\n3- Criar playlist");
    printf("\n4- Inserir musica na playlist");
    printf("\n5- Imprimir playlists criadas");
    printf("\n6- Imprimir musicas das playlists criadas");
    printf("\n7- Excluir");
    printf("\n8- Shuffle");
    printf("\n0- Sair\n");
    scanf("%d", &opcaoInicio);

    return opcaoInicio; // Retorna a opção digitada pelo usuário
}

int altura_NO(struct NO* no){ // funcao para saber a altura do no
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int fatorBalanceamento_NO(struct NO* no){//funcao para fator de balanceamento
    return labs(altura_NO(no->esq) - altura_NO(no->dir)); //long int abs = "como se fosse o modulo do resultado da operacao"
}

int maior(int x, int y){ //saber qual o maior (usado na funcao de altura do no, exclusao, insercao, rotacao dos nos)
    if(x > y)
        return x;
    else
        return y;
}


//===============ROTACOES==================
void RotacaoLL(ArvAVL *A){//LL SIMPLES dir   "cada no sobe um a direita, como a raiz nao pode subir mais, raiz desce 1 a direita obs: exemplo com 3 numeros".
    printf("RotacaoLL\n");
    struct NO *B;
    B = (*A)->esq; 
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior( altura_NO((*A)->esq) , altura_NO( (*A)->dir) ) + 1;
    B->altura = maior(  altura_NO(B->esq),  (*A)->altura) + 1;
    *A = B;
}

void RotacaoRR(ArvAVL *A){//RR SIMPLES esq   "cada no sobe um a esquerda, como a raiz nao pode subir mais, raiz desce 1 a esquerda obs: exemplo com 3 numeros".
    printf("RotacaoRR\n");
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

void RotacaoLR(ArvAVL *A){//LR DUPLA esq dir     "no sobe um pra direita e depois sobe a esquerda trocando com a raiz (exemplo com 3 nos)"".
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

void RotacaoRL(ArvAVL *A){//RL DUPLA dir esq     "no sobe um pra esquerda e depois sobe a direita trocando com a raiz (exemplo com 3 nos)"."
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

int insere_ArvAVL(ArvAVL *raiz, musica *valor){
    int res;
    if(*raiz == NULL){//arvore vazia ou no folha
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO)); //aloca espaco para o no
        if(novo == NULL)
            return 0;

        novo->info = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    struct NO *atual = *raiz;     
    if(valor->id < atual->info->id){  //comparar o novo no id com o no que percorreu na arv
        if((res = insere_ArvAVL(  &(atual->esq) , valor) ) == 1){ //novo no id e menor entao ocupa a esq
            if(fatorBalanceamento_NO(atual) >= 2){ //verifica se esta dentro do fator de balanceamento
                if(valor->id <  (*raiz)->esq->info->id  ){ //caso n esteja dentro do fat balanceamento = balancear arv
                    RotacaoLL(raiz); //caso id seja menor que o no da esq
                }else{
                    RotacaoLR(raiz); //caso id seja maior que o no da esq
                }
            }
        }
    }else{
        if(valor->id > atual->info->id){  //comparar o novo no id com o no que percorreu na arv
            if((res = insere_ArvAVL(&(atual->dir), valor)) == 1){  //novo no id e maior entao ocupa a dir
                if(fatorBalanceamento_NO(atual) >= 2){ //verifica se esta dentro do fator de balanceamento
                    if((*raiz)->dir->info->id < valor->id){ //caso n esteja dentro do fat balanceamento = balancear arv
                        RotacaoRR(raiz); //caso o no da direita seja menor que o novo no
                    }else{
                        RotacaoRL(raiz);//caso o no da direita seja maior que o novo no
                    }
                }
            }
        }else{
            printf("Valor duplicado!!\n");
            return 0;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1; //define altura do novo no, comparando as duas alturras dos galhos

    return res;
}

//FUNCAO CADASTRA MUSICA
void cadastraMusica(ArvAVL *raiz)
{   
    system("cls"); //Limpa o terminal
    printf("\n----------CADASTRO DE MUSICA----------\n");

    //variaveis
    int hr, min, seg, id;

    musica *c; /* o ponteiro para o espaço alocado */

    /* aloco um único byte na memória */
    c = malloc(sizeof(musica));

    printf("Id da musica: ");
    scanf(" %d", &id);
    c->id = id;

    printf("Nome da musica: ");
    scanf(" %[^\n]s", c->titulo); //Atribui o que foi digitado ao titulo de C

    printf("Nome do artista: ");
    scanf(" %[^\n]s", c->artista); //Atribui o que foi digitado ao arista de C

    printf("Nome do album: ");
    scanf(" %[^\n]s", c->album); //Atribui o que foi digitado ao album de C

    printf("Quantidade de horas da musica: ");
    scanf("%d", &hr); //Horas de música

    printf("Quantidade de minutos da musica: ");
    scanf("%d", &min); //Minutos de musica

    printf("Quantidade de segundos da musica: ");
    scanf("%d", &seg); //Segundos de música

    //conversao do tempo da musica para segundos
    c->duracao = converterSegundos(hr, min, seg);

    //Insere C (a música) na arvore
    insere_ArvAVL(raiz, c);
    
    system("pause");
}

//FUNCAO IMPRIME MUSICAS AVL
ArvAVL imprime(ArvAVL *raiz){
    if(*raiz == NULL) 
        return;
    if(*raiz != NULL){
        imprime(&((*raiz)->esq));
        //printf("%d\n",(*raiz)->info);
        printf("\n%d | %s | %s | %s | ", (*raiz)->info->id, (*raiz)->info->titulo, (*raiz)->info->artista, (*raiz)->info->album); //Imprime as informações da música
        converterHora((*raiz)->info->duracao);
        printf("\nNo %d: H(%d) fb(%d)\n",(*raiz)->info->id,altura_NO(*raiz),fatorBalanceamento_NO(*raiz));
        imprime(&((*raiz)->dir));
    }
    return raiz;
}

//FUNÇÃO CRIA PLAYLIST
int criaPlaylist(int id1)
{
    system("cls");
    //soma contador de id da Playlist
    id1++;
    //criacao cabeca para lista ligada circular de playlist
    iniPlaylist = malloc(sizeof(playlist_no)); //Aloca espaço na memória
    iniPlaylist->prox = iniPlaylist;           //o próx de iniPlaylist recebe iniPlaylist
    char nome[200];
    printf("\n----------CRIAR PLAYLIST----------\n");
    printf("Digite o nome da playlist: ");
    scanf(" %[^\n]s", &nome); //Nome da Playlist

    inserePlaylistPasta(id1, nome, iniPlaylist); //Insere Playlist na Pasta de Playlists

    return id1;
    system("pause");
}

//FUNÇÃO INSERE PLAYLIST NA PASTA
void inserePlaylistPasta(int id, char nome[200], playlist_no *p)
{
    lplaylists_no *novo;                  //Cria um novo ponteiro do tipo lplaylist_mp
    novo = malloc(sizeof(lplaylists_no)); //Aloca espaço na memória para novo
    novo->id = id;                        //O ID de novo recebe ID da playlist
    strcpy(novo->nome, nome);             //Nome de novo é igual a Nome *(Essa função foi utilizada devido a erros na atribuição devido ao tipo)*
    novo->musicas = p;                    //Musicas de novo recebe p
    novo->prox = iniPastaPlaylist->prox;  //Próximo de novo recebe o próximo da cabeça
    iniPastaPlaylist->prox = novo;        //Próximo da cabeça recebe novo
}

//FUNÇÃO CONVERTE EM SEGUNDOS
int converterSegundos(int hr, int min, int seg)
{
    return (hr * 3600) + (min * 60) + seg; //Retorna em segundos
}

//FUNÇÃO CONVERTE NO FORMATO CORRETO
int converterHora(int segNconvertido)
{
    int hr, min, segundos, resto; //horas, minutos, segundos, resto

    resto = segNconvertido % 3600; //Resto para prosseguir com os demais cálculos
    hr = segNconvertido / 3600;    //Cálculo para converter os segundos em hora

    min = resto / 60;   //Cálculo para converter os segundos restantes em minutos
    resto = resto % 60; //Resto para prosseguir com os demais cálculos

    segundos = resto; //Segundos de música

    printf("%.2d:%.2d:%.2d", hr, min, segundos); //Imprime Tempo formatado
}

//FUNÇÃO BUSCA PLAYLIST
int buscaPlaylist()
{
    system("cls");
    char nome[200];
    printf("Digite o nome da playlist: ");
    scanf(" %[^\n]", &nome);    //Nome da Playlist
    lplaylists_no *p;           //Cria um novo ponteiro do tipo lPlaylists_no
    p = iniPastaPlaylist->prox; //p recebe o próximo da cabeça
    while (p)
    {
        if (strcmp(p->nome, nome) == 0) //Se o nome da playlist atual for igual ao nome digitado
        {
            return p->musicas; //true
        }
        p = p->prox; //p recebe o próximo
    }
}

//FUNÇÃO INSERE MÚSICA NA PLAYLIST
void insereMusicaPlaylist(int id1, ArvAVL *raiz)
{
    system("cls");
    if (id1 > 0 || raiz == NULL) //Se id de Playlist for maior que 0 ou a raiz estiver vazia
    {
        int verificador1;
        verificador1 = buscaPlaylist(); //Busca a Playlist e recebe seu endereço de memória

        if (verificador1 != NULL) //Se verificador for diferente de nulo
        {
            printf("Digite os ids das musicas para serem adicionadas (de 3 em 3): ");

            //biblioteca strtok();
            int vetor[3];
            char nums[512];                  //espaço para os numeros todos como um texto
            scanf(" %[^\n]", nums);          //lê todos os números como um texto
            char *token = strtok(nums, " "); //lê até ao primeiro espaço para a variavel token
            int pos = 0;                     //posição para guardar os numeros no vetor começa em 0

            //biblioteca atoi()
            while (token != NULL && pos < 3)
            {
                vetor[pos++] = atoi(token); //guarda o valor convertido em numero no vetor e avança
                token = strtok(NULL, " ");  //lê até ao próximo espaço a partir de onde terminou
            }

            int i, verificador;
            struct NO p ;
           

         

            for (i=0;i<3;i++){
                struct NO* atual = *raiz;
                verificador=0;
                while(atual != NULL){
                    if(vetor[i] == atual->info->id){
                        printf("\n%d | %s | %s | %s | ", atual->info->id, atual->info->titulo, atual->info->artista, atual->info->album); //Imprime as informações da música
                        converterHora(atual->info->duracao);
                        verificador=1;
                        printf("\n");
                        alocaMusicaPlaylist(atual->info, verificador1);
                    }
                    if(vetor[i] > atual->info->id){
                        atual = atual->dir;
                    }
                    else{
                        atual = atual->esq;
                    }
                }
                if (verificador == 0) printf("\nMusica ID: %d nao encontrado!\n", vetor[i]);

            }

        }
        else
        {
            printf("\nNome da playlist nao encontrada\n");
        }
    }
    else
    {
        printf("\nNenhuma musica cadastrada ou nenhuma playlist cadastrada!\n");
    }
    system("pause");
}

//FUNÇÃO INSERE MÚSICA NA PLAYLIST
void alocaMusicaPlaylist(musica *p, playlist_no *q)
{
    playlist_no *novo;                  //Cria um novo ponteiro do tipo playlist_no
    novo = malloc(sizeof(playlist_no)); //Aloca espaço na memória para esse ponteiro
    novo->musica = p;                   //Novo recebe p como música
    novo->prox = q->prox;               //O próximo de novo recebe o primeiro item da playlist
    q->prox = novo;                     //O primeiro item da playlist recebe novo
}

//FUNÇÃO IMPRIME LISTA DE PLAYLISTS
void imprimePlaylistPasta(int id)
{
    system("cls");
    if (id > 0) //Se existir ao menos uma playlist
    {
        printf("\n----------LISTA DE PLAYLISTS----------\n");
        lplaylists_no *p;           //Cria um novo ponteiro do tipo lplaylist_no
        p = iniPastaPlaylist->prox; //p recebe o próximo da cabeça
        while (p)
        {
            printf("\n%d | %s \n", p->id, p->nome); //Imprime informações da playlist atual
            p = p->prox;                            //p recebe o próximo
        }
    }
    else
    {
        printf("\nNenhuma playlist cadastrada\n");
    }
    system("pause");
}

//FUNÇÃO IMPRIME MÚSICAS DA PLAYLIST
void imprimeMusicaPlaylist(int id)
{
    system("cls");
    if (id > 0) //Se existir ao menos uma playlist
    {
        char nome[200];
        printf("\n----------LISTA DE MUSICAS NA PLAYLIST----------\n");
        printf("Digite o nome da playlist para exibir as musicas: ");
        scanf(" %[^\n]s", &nome); //Nome da playlist

        int verificador = 0; //para verificar a existência da música

        lplaylists_no *p; //Cria um novo ponteiro p do tipo lplaylist_no
        lplaylists_no *q; //Cria um novo ponteiro q do tipo lplaylist_no

        p = iniPastaPlaylist->prox; //p recebe próximo da cabeça
        while (p)                   //Enquanto não chegar ao fim da lista
        {
            if (strcmp(p->nome, nome) == 0) //Se o nome da playlist atual for igual ao nome digitado
            {
                verificador = 1; // true
                q = p->musicas;  // q recebe as músicas dessa playlist
            }
            p = p->prox; // p recebe o próximo da lista
        }
        if (verificador == 1) //Se for verdade
        {
            imprimeDentroPlaylist(q, nome); //Imprime as músicas presentes na playlist
        }
        else
        {
            printf("Playlist nao encontrada");
        }
    }
    else
    {
        printf("Nenhuma playlist cadastrada");
    }
    system("pause");
}

//FUNÇÃO IMPRIME MÚSICAS DA PLAYLIST 2
void imprimeDentroPlaylist(playlist_no *p, char nome[200])
{
    system("cls");
    printf("\n----------PLAYLIST: %s ----------\n", nome);
    playlist_no *q; //Cria um novo ponteiro do tipo Playlist_no
    q = p->prox;    //q recebe a primeiro item da playlist
    while (q != p)  //Enquanto q for diferente de p
    {
        printf("\n%d | %s | %s | %s | ", q->musica->id, q->musica->titulo, q->musica->artista, q->musica->album); //Imrpime as informações da playlist
        converterHora(q->musica->duracao);                                                                        //Converte a duração para o formato Hor:Min:Seg
        printf("\n");
        q = q->prox; //q recebe o próximo da lista
    }
}

//FUNCAO PARA ACHAR O MENOR VALOR DA FOLHA DA AVL
struct NO* procuraMenor(struct NO* atual){ //achar o menor no 
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

//FUNCAO REMOVE MUSICA DA AVL
int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){// valor nao existe
	    printf("Arvore VAZIA\n");
	    return 0;
	}

    int res;

    //balancear arv 
	if(valor < (*raiz)->info->id){ //valor menor que a raiz
	    if( (res = remove_ArvAVL( &(*raiz)->esq , valor) ) == 1){ //recebe o no da esq
            if(fatorBalanceamento_NO(*raiz) >= 2){ //caso fat balanceamento n esteja balanceado
                if(altura_NO((*raiz)->dir->esq) <= altura_NO((*raiz)->dir->dir)) //altura do no a direita depois esquerda <= que altura no direita depois direita
                    RotacaoRR(raiz);
                else
                    RotacaoRL(raiz);
            }
	    }
	}

    //balancear arv 
	if((*raiz)->info->id < valor){ //raiz menor que valor
	    if((res = remove_ArvAVL(&(*raiz)->dir, valor)) == 1){ //recebe o no da direita
            if(fatorBalanceamento_NO(*raiz) >= 2){ //caso fat balanceamento n esteja balanceado
                if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq) )//altura do no a esquerda depois direita <= que altura no esquerda depois esquerda
                    RotacaoLL(raiz);
                else
                    RotacaoLR(raiz);
            }
	    }
	}

	if((*raiz)->info->id == valor){
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){// no tem 1 filho ou nenhum
			struct NO *oldNode = (*raiz); //armazena o no a ser removido
			if((*raiz)->esq != NULL) //no filho a esquerda
                *raiz = (*raiz)->esq;
            else //no filho a direita
                *raiz = (*raiz)->dir;
			free(oldNode);//libera o NO que sera excluido
		}else { // no tem 2 filhos
			struct NO* temp = procuraMenor((*raiz)->dir); //achar o menor no filho dentre os maiores
			(*raiz)->info = temp->info; //raiz info vai receber o valor do menor filho dentre os maiores
			remove_ArvAVL(&(*raiz)->dir, (*raiz)->info->id); //chama funcao para poder balancear a arv com o valor do menor no dentre os maiores junto com o filho a direita do proprio no
            if(fatorBalanceamento_NO(*raiz) >= 2){
				if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
					RotacaoLL(raiz);
				else
					RotacaoLR(raiz);
			}
		}
		if (*raiz != NULL)
            (*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1; // acha a maior altura entre dois nos
		return 1;
	}

	(*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;

	return res;
}

//FUNCAO REMOVE PLAYLIST
void removerPlaylist(int id)
{
    int enderecoPlaylist;
    enderecoPlaylist = buscaPlaylist(); //Busca a Playlist e recebe seu endereço de memória

    playlist_no *p = enderecoPlaylist; //Cria um novo ponteiro p do tipo playlist_no e recebe endereço de memória
    playlist_no *q = enderecoPlaylist; //Cria um novo ponteiro q do tipo playlist_no e recebe endereço de memória
    p = p->prox;                       //p recebe o próximo para ficar a frente do q
    while (p != enderecoPlaylist)      //Enquanto p for diferente do inicio da playlist
    {
        if (p->musica->id == id) //Se o id da música e o id digitado forem iguais
        {
            q->prox = p->prox; //Exclui a música da playlist
        }
        else
        {
            q = q->prox; //q recebe o próximo
        }
        p = p->prox; //p recebe o próximo
    }
}

//FUNCAO REMOVE DA AVL E DA PLAYLIST
void removerTudo(ArvAVL* raiz, int valor){
    lplaylists_no *q;
    playlist_no *r, *s;
    q = iniPastaPlaylist->prox;
    while (q)
    { // Percorre PastaPlaylist
        r = q->musicas->prox;
        s = q->musicas;
        while (r != q->musicas)
        { // Percorre Playlist
            if (r->musica->id == valor)
            {                      // Localizou a música
                s->prox = r->prox; //a musica da playlist é excluida
            }
            else
            {
                s = s->prox; // Vai para a próxima
            }
            r = r->prox; //Vai para a próxima
        }
        q = q->prox; //Vai para a próxima
    }
    remove_ArvAVL(raiz, valor); //funcao para tirar da arv
}

//FUNÇÃO ACHA VALORES PARA TROCA
int acharTroca(int playlist, int count)
{
    int x;

    x = (rand() % count + 1); //Seleciona um número randomizado dentre a quantidade de musicas exitentes na playlist

    int contador;
    contador = 0;
    playlist_no *p, *q;
    p = playlist;         // p recebe playlist
    while (contador <= x) //Enquanto o contador for menor ou igual ao número randomizado
    {
        if (contador == x)
        {
            return p; //retorna a música selecionada pela randomização
        }
        p = p->prox; //p recebe próximo
        contador++;  // adiciona 1 ao contador
    }
}

//FUNÇÃO TROCA
void troca(playlist_no *q, playlist_no *r)
{
    playlist_no *aux;
    aux->musica = q->musica; //aux recebe 1º musica
    q->musica = r->musica;   // q recebe 2º musica
    r->musica = aux->musica; // r recebe 1º musica
}

//FUNÇÃO RETORNA QUANTIDADE DE MÚSICAS NA PLAYLIST
int qntdMusicaPlaylist(playlist_no *r)
{
    int count = 1;
    if (r != NULL)
    {
        playlist_no *q, *p;
        p = r;       // p recebe a playlist
        q = p->prox; // q recebe o primeiro item da playlist
        while (q != p)
        {
            count++;     //Adiciona 1 ao contador
            q = q->prox; //q recebe o próximo
        }
        return count; //retorna contadador com a quantidade músicas na playlist
    }
    else
    {
        return count = 0;
    }
}

//FUNÇÃO SHUFFLE
void shuffle()
{
    int playlist;
    int count;
    playlist = buscaPlaylist();           //Busca playlist e retorna seu endereço de memória
    count = qntdMusicaPlaylist(playlist); // Define a quantidade de músicas na playlist

    count -= 1; //Contador reduzido para não interferir na randomização

    int i = 1;

    if (count > 0)
    {

        playlist_no *w;
        w = playlist; // w recebe playlist
        w = w->prox;  // w recebe o primeiro item da playlist
        printf("------------1--------------");
        while (w != playlist)
        {
            printf("\n%d | %s | %s | %s | ", w->musica->id, w->musica->titulo, w->musica->artista, w->musica->album); //imprime os dados das músicas antes do shuffle
            converterHora(w->musica->duracao);                                                                        //converte a duração para Hor:Min:Seg
            printf("\n");
            w = w->prox; //w recebe o próximo
        }

        playlist_no *q, *r;

        while (i <= count / 2)
        {
            q = acharTroca(playlist, count); //Acha uma música aleatória para realizar a troca
            r = acharTroca(playlist, count); //Acha outra música aleatória para realizar a troca
            troca(q, r);                     //realiza a troca
            i++;
        }

        w = playlist; // w recebe playlist
        w = w->prox;  // w recebe o primeiro item da playlist
        printf("------------2--------------");
        while (w != playlist)
        {
            printf("\n%d | %s | %s | %s | ", w->musica->id, w->musica->titulo, w->musica->artista, w->musica->album); // imprime os dados das músicas depois do shuffle
            converterHora(w->musica->duracao);                                                                        //converte a duração para Hor:Min:Seg
            printf("\n");
            w = w->prox; //w recebe o próximo
        }
    }
    else
    {
        printf("Playlist nao encontrada");
    }

    system("pause");
}


//FUNÇÃO PRINCIPAL
int main()
{
    system("cls");     //Limpa o terminal
    srand(time(NULL)); //Insere uma "semente" (utilizada devido da randomização presente na função shuffle())
    int startMenu;     //para o menu funcionar com o while
    //start contador de id playlist
    int idPlaylist = 0;
    
    //criacao cabeca para lista ligada pasta de playlist
    iniPastaPlaylist = malloc(sizeof(lplaylists_no)); //Aloca espaço na memória
    iniPastaPlaylist->prox = NULL;                    //Inicia prox em NULL

    //criar ARVORE
    ArvAVL* raizGlobal;
    raizGlobal = cria_ArvAVL();

    int opc; //para a escolha de remoção ser mais precisa
    
    while (startMenu)
    {
        startMenu = menuInicio(); //Retorna a startMenu a opção do usuário

        switch (startMenu) //reproduz o que foi solicitado pela escolha do usuário
        {
        case 1:
            cadastraMusica(raizGlobal);
            break;

        case 2:
            system("cls");
            imprime(raizGlobal);//IMPRIME MUSICAS
            system("pause");
            break;

        case 3:
            idPlaylist = criaPlaylist(idPlaylist);
            break;

        case 4:
            insereMusicaPlaylist(idPlaylist, raizGlobal);
            break;

        case 5:
            imprimePlaylistPasta(idPlaylist);
            break;

        case 6:
            imprimeMusicaPlaylist(idPlaylist);
            break;
        
        case 7:
            system("cls");
            printf("\n1- Excluir musica da playlist\n2- Excluir musica\nEscolha: ");
            scanf("%d", &opc); //Armazena a opção de remoção

            if (opc == 1)
            {
                int idRemovePlaylist;
                printf("\nId da musica a ser excluida: ");
                scanf("%d", &idRemovePlaylist); //ID da música escolhida para ser excluida da Playlist

                removerPlaylist(idRemovePlaylist);
            }
            else if (opc == 2)
            {
                int idRemoveTudo;
                system("cls");
                printf("\nId da musica a ser excluida: ");
                scanf("%d", &idRemoveTudo); //ID da música escolhida para ser excluida de todos os lugares possíveis

                removerTudo(raizGlobal, idRemoveTudo);
            }
            else
            {
                printf("\nOpcao invalida!\n");
                system("pause");
            }
            break;

        case 8:
            system("cls");
            shuffle();
            break;
        }
        system("cls");
    }
    printf("\nPROGRAMA ENCERRADO!");
}