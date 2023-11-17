/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keymap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYMAP_H
# define KEYMAP_H

# define BUTTONPRESS	4
# define BUTTONRELEASE 5
# define MOTIONNOTIFY 6
# define BUTTONPRESSMASK	2
# define BUTTONRELEASEMASK 3
# define POINTERMOTIONMASK 6

# ifdef LINUX
#  define K_ESC 65307
#  define K_UP 65362
#  define K_DOWN 65364
#  define K_LEFT 65361
#  define K_RIGHT 65363
#  define K_R 114
#  define K_TAB 65289
#  define K_SPACE 32
#  define K_SHIFT 65505
#  define K_CTRL 65507
#  define K_ENTER 65293
#  define K_0 48
#  define K_1 49
#  define K_2 50
#  define K_3 51
#  define K_4 52
#  define K_5 53
#  define K_6 54
#  define K_7 55
#  define K_8 56
#  define K_9 57 
#  define K_NP_0 65438
#  define K_NP_1 65436
#  define K_NP_2 65433
#  define K_NP_3 65435
#  define K_NP_4 65430
#  define K_NP_5 65437
#  define K_NP_6 65432
#  define K_NP_7 65429
#  define K_NP_8 65431
#  define K_NP_9 65434
#  define K_A 97
#  define K_B 98
#  define K_C 99
#  define K_D 100
#  define K_E 101
#  define K_F 102
#  define K_G 103
#  define K_H 104
#  define K_I 105
#  define K_J 106
#  define K_K 107
#  define K_L 108
#  define K_M 109
#  define K_N 110
#  define K_O 111
#  define K_P 112
#  define K_Q 113
#  define K_R 114
#  define K_S 115
#  define K_T 116
#  define K_U 117
#  define K_V 118
#  define K_W 119
#  define K_X 120
#  define K_Y 121
#  define K_Z 122
#  define K_PLUS 61
#  define K_MINUS 45
# else
#  define K_ESC 53
#  define K_UP 126
#  define K_DOWN 125
#  define K_LEFT 123
#  define K_RIGHT 124
#  define K_R 15
#  define K_TAB 48
#  define K_SPACE 49
#  define K_SHIFT 257
#  define K_CTRL 256
#  define K_ENTER 36
#  define K_0 29
#  define K_1 18
#  define K_2 19
#  define K_3 20
#  define K_4 21
#  define K_5 23
#  define K_6 22
#  define K_7 26
#  define K_8 28
#  define K_9 25
#  define K_NP_0 82
#  define K_NP_1 83
#  define K_NP_2 84
#  define K_NP_3 85
#  define K_NP_4 86
#  define K_NP_5 87
#  define K_NP_6 88
#  define K_NP_7 89
#  define K_NP_8 91
#  define K_NP_9 92
#  define K_A 0
#  define K_B 11
#  define K_C 8
#  define K_D 2
#  define K_E 14
#  define K_F 3
#  define K_G 5
#  define K_H 4
#  define K_I 34
#  define K_J 38
#  define K_K 40
#  define K_L 37
#  define K_M 46
#  define K_N 45
#  define K_O 31
#  define K_P 35
#  define K_Q 12
#  define K_R 15
#  define K_S 1
#  define K_T 17
#  define K_U 32
#  define K_V 9
#  define K_W 13
#  define K_X 7
#  define K_Y 16
#  define K_Z 6
#  define K_PLUS 27
#  define K_MINUS 24
# endif

# define K_MOVE K_G
# define K_ROTATE K_R
# define K_SCALE K_S

# define K_RENDERMODE K_O
# define K_MATMODE K_P

# define K_PLANE_X K_X
# define K_PLANE_Y K_Z
# define K_PLANE_Z K_Y

# define K_MOTION K_I
# define K_LOCKON K_L
# define K_HELPON K_H

# define K_REPRINT K_ENTER

//adds
//1-4 objects
//5-6 lamps

//move  
//arrows- forward, backward
//  ''  - right, left
//  QE  - up down

//numpad

//ctrl - snap


#endif