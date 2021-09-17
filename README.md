# Thesis

Arduino, no "SETUP" temos os comandos para ver os valores de STATUS, as definicoes dos diferentes rails. e a leitura de valores como corrente e tensoes.

O que importa para testar as diferentes tensoes para as diferentes  instruccoes, esta no "Loop".

So importa ver os SW-B(1) e SW-D(3) do REG2.

Para os diferentes funcionamentos sao usados os diferentes #defines, combinacoes de modo a nao ocupar totalmente a memoria dinamica do Arduino.

"check_outputs_config" verifica as definicoes dos diferentes rails, usado com o "non_important_pins_only" ou "important_pins" e com "other_half" ou sem.

"check_device config" verifica as definicoes dos comandos.

"check_status_init" verifica os comandos status que exibem os erros e warnings.

"check_output" verifica os valores de telemetria dos diferentes rails, corrente, potencia, tensao e temperatura. Usado com "non_important_pins_only" ou "important_pins" e se pertinente com non_important_pins_only que restringe ao REG2.

"check_prints_clean" usado para ver o recebido por I2C antes de processado, para debug.

"routine" é o usado para alterar a tensão.
