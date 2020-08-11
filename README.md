# Arduino Nano Luminaria

## Descrição
Projeto de luminária para aromaterapia com LEDs e nebulizador ultrassonico.

## Hardware
Para o projeto o escolhido foi um Arduino Nano.

## Pendências
- Adicionar push button para controle dos LEDs.
    > 1 toque liga em uma cor fixa, outro toque troca a cor. Um toque longo muda para o modo aleatório e outro toque longo desliga os LEDs.

- Ressonador trabalha a 2,4MHz, precisa verificar se timer do Arduino permite trabalhar nessa frequência;

- Detecção de falta de água ou nível baixo.
    > Ver exemplos dos CIs BS45F3832 e BS45F3833. Dá pra usar?

- Adicionar push button pra controle do nebulizador.
   > 1 toque liga com potência baixa, outro toque liga com potência alta e toque longo desliga.

- Adicionar buzzer pra cada acionamento dos botões e indicação de falta de água.
