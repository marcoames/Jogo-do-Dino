/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * Custom char code by Marcelo Cohen - 2021
 */

#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"

uint8_t road[] =  { 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000000 };
uint8_t dino[] = { 0b00000000, 0b11111100, 0b11111010, 0b00101110, 0b00001110 };
uint8_t obstaculo[] =  { 0b00000000, 0b00101000, 0b11111000, 0b00101000, 0b00000000 };
uint8_t branco[] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };

int pulo (int z) {

    // branco na road
    nokia_lcd_set_cursor(8,34);
    nokia_lcd_write_string("\004", 1);
    nokia_lcd_render();

    // dino pula
    nokia_lcd_set_cursor(8,28);
    nokia_lcd_write_string("\002", 1);
    nokia_lcd_render();
    //_delay_ms(250);

    // faz o obstaculo andar com o dino no ar
    for (int x = z; x > z-12; x --) {

        // obstaculo na road
        nokia_lcd_set_cursor(x,35);
        nokia_lcd_write_string("\003",1);
        nokia_lcd_render();
        _delay_ms(10);

    }

    // branco no pulo
    nokia_lcd_set_cursor(8,28);
    nokia_lcd_write_string("\004", 1);
    nokia_lcd_render();

    // dino na road
    nokia_lcd_set_cursor(8,34);
    nokia_lcd_write_string("\002", 1);
    nokia_lcd_render();

    // faz poder pular depois de ja ter pulado 1 vez
    int x = z-12;
    depoisPulo(x);

}

int depoisPulo(int z) {

    // obstaculo percorrendo a road
    for (int x = z; x > 0; x --) {

        // obstaculo na road
        nokia_lcd_set_cursor(x,35);
        nokia_lcd_write_string("\003",1);
        nokia_lcd_render();
        _delay_ms(10);

        if (PINB & (1 << PB0)) {    // botao pulo pressionado
            pulo(x);
            break;
        }

        // se colide com obstaculo
        if (x > 4 && x <12) {
            end();
            break;
        }

    }

}

int start(void) {

    nokia_lcd_clear();
    nokia_lcd_set_cursor(14,12);
    nokia_lcd_write_string("START", 2);
    nokia_lcd_render();
    _delay_ms(1000);
    nokia_lcd_clear();

    // score
    int score1 = 0; // decimal
    int score2 = 0; // unitario

    while(1) {

        // DISPLAY SCORE
        nokia_lcd_set_cursor(36,0);
        nokia_lcd_write_string("SCORE:", 1);
        nokia_lcd_set_cursor(72,0);
        nokia_lcd_write_char('0'+ score1, 1);
        nokia_lcd_set_cursor(78,0);
        nokia_lcd_write_char('0'+ score2, 1);
        nokia_lcd_render();

        // dino parado na road
        nokia_lcd_set_cursor(8,34);
        nokia_lcd_write_string("\002", 1);
        nokia_lcd_render();

        // faz road correr para esquerda
        for(int x = 83; x > 0; x--) {

            nokia_lcd_set_pixel(x,42,1);
            nokia_lcd_render();
            //_delay_ms(10);

            // coloca obstaculo na road correndo para esquerda
            if (x == 1) {

                for (int z = 80; z>0; z--) {

                    nokia_lcd_set_cursor(z,35);
                    nokia_lcd_write_string("\003",1);
                    nokia_lcd_render();
                    _delay_ms(10);

                    if (PINB & (1 << PB0)) {    // botao pulo pressionado
                        pulo(z);
                        break;
                    }

                    if (PINB & (1 << PB6)) {    // RESTART
                        end();
                        break;
                    }

                    // se colide com obstaculo
                    if (z > 4 && z <12) {
                        end();
                        break;
                    }

                }

                //limpa o obstaculo no comeco da road
                nokia_lcd_set_cursor(1,35);
                nokia_lcd_write_string("\004",1);
                nokia_lcd_render();

            }
           
        }

        // incrementa score
        score2 = score2 + 1;
        if (score2 > 9) {
            score1 += 1;
            score2 = 0;
        }

    }

}

int end() {

    // INTERFACE FIM DO JOGO
    nokia_lcd_clear();
    nokia_lcd_set_cursor(15,8);
    nokia_lcd_write_string("GAME OVER", 1);
    nokia_lcd_set_cursor(0,28);
    nokia_lcd_write_string("PRESS TO START",1);
    nokia_lcd_render();
    _delay_ms(1000);
    
    // Botao para restart no jogo
    while(1) {

        if (PINB & (1 << PB6)) {    // RESTART
            start();
        }

    }

}

int main(void) {

    DDRB &= ~(1 << PB0); // PB0 como entrada
    DDRB &= ~(1 << PB6); // PB6 como entrada
    
    nokia_lcd_init();
    nokia_lcd_clear();

    nokia_lcd_custom(1,road);       // custom 1 road
    nokia_lcd_custom(2,dino);       // custom 2 dino
    nokia_lcd_custom(3,obstaculo);  // custom 3 obstaculo
    nokia_lcd_custom(4,branco);     // custom 4 branco

    // MENU PRINCIPAL
    nokia_lcd_set_cursor(20,8);
    nokia_lcd_write_string("DINO", 2);
    nokia_lcd_set_cursor(0,32);
    nokia_lcd_write_string("PRESS TO START",1);
    nokia_lcd_render();

    while (1) {
    
        // COMECO DO JOGO
        if (PINB & (1 << PB6)) {    // botao de start pressionado
            start();
        }

    }

}