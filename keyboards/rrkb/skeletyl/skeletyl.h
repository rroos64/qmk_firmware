/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "quantum.h"

// clang-format off
#define LAYOUT_split_3x5_3(                                       \
  l00, l01, l02, l03, l04, l05,     r05, r04, r03, r02, r01, r00, \
  l10, l11, l12, l13, l14, l15,     r15, r14, r13, r12, r11, r10, \
  l20, l21, l22, l23, l24, l25,     r25, r24, r23, r22, r21, r20  \
)                                                                 \
{                                                                 \
  {   l00,   l01,   l02,   l03,   l04,  l05 },                    \
  {   l10,   l11,   l12,   l13,   l14,  l15 },                    \
  {   l20,   l21,   l22,   l23,   l24,  l25 },                    \
  {   r00,   r01,   r02,   r03,   r04,  r05 },                    \
  {   r10,   r11,   r12,   r13,   r14,  r15 },                    \
  {   r20,   r21,   r22,   r23,   r24,  r25 }                     \
}
// clang-format on
