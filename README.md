
serao no minimo 3 threads

1 thread que gera trabalhos
1 thread que trabalha
1 thread que exibe o resultado
2 buffers como serao os buffers?

idealizando

cria ai 10 trabalhos e usa 2 threads para resolver
serao 4 threads

cria(10, 2)
  ok, quais sao os 10 trabalhos
  o trablho é uma informacao tipo um x e y
  a thread principal insere os trabalhos no buffer de trabalho
  mas como a thread principal sabe quais os trabalhos?
  ele pode receber um buffer de trabalhos a executar ou porde gerar os seu buffer de trabalhos
  acho que gerar o seu buffer de trabalhos seria o mais interessantes
  como gerar seu buffer de trabalhos
  na verdade isto nao precisa ser a thread principal que gera
  pode ser o programa principal
  no caso, o trabalho é um pedaco da tela eh sem graca gerar um buffer de tamanho exato
  supomos que tenho 150 tarefas, ou seja, dividi a tela em 150 pedacos
  crio um buffer de tamanho 10
  e crio 2 threads trabalhadoras

  criar uma classe chamada buffers que seja responsavel pelos buffers workPool e donePool, com alguns metodos para auxiliar OK
  criar uma classe work que lide com os dois buffers ok

  separar classes de buffers
  criar consumidor que sera consumidor do buffer de trabalho e produtor do buffer de saida