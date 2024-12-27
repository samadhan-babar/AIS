/*
  Copyright (c) 2016-2020 Peter Antypas

  This file is part of the MAIANA™ transponder firmware.

  The firmware is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>
*/

/**
 * @file _assert.h
 * @brief Header file for assertion macros in the Marine Automatic Identification System firmware.
 * @details This file provides macros for debugging assertions, used to validate conditions and pointers during debugging.
 *          The assertions are only enabled in debug mode, and they trigger a breakpoint if the condition fails.
 * @version 1.0A
 * @date September 2024
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef DEBUG
/**
 * @brief Macro to assert a condition.
 * @details In debug mode, this macro checks if the given condition is true. If the condition is false,
 *          it triggers a breakpoint (`bkpt 0`) to halt the execution.
 * @param b The condition to be asserted.
 */
#define ASSERT(b) if (!(b)) {asm("bkpt 0");}
#else
/**
 * @brief Macro for release mode where assertions are disabled.
 * @param b The condition (ignored in release mode).
 */
#define ASSERT(b)
#endif

#ifdef DEBUG
/**
 * @brief Macro to assert that a pointer is valid.
 * @details This macro ensures that the pointer `p` is within the valid memory range for pointers (0x20000000 to 0x20009FFF).
 *          It is only active in debug mode and triggers a breakpoint if the pointer is invalid.
 * @param p The pointer to be validated.
 */
#define ASSERT_VALID_PTR(p) ASSERT((uint32_t)p >= 0x20000000 && (uint32_t)p <= 0x20009FFF)
#else
/**
 * @brief Macro for release mode where pointer assertions are disabled.
 * @param p The pointer (ignored in release mode).
 */
#define ASSERT_VALID_PTR(p)
#endif

#endif /* ASSERT_H_ */

// Local Variables:
// mode: c++
// End:
