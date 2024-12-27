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
 * @file CircularQueue.hpp
 * @brief Defines a template-based circular queue class for managing buffered data.
 * @details This file provides an implementation of a circular queue (or ring buffer) using a template,
 *          which allows for storing and managing elements in a buffer of fixed size. The queue supports push and pop operations
 *          and is commonly used in buffering scenarios like streaming data.
 * @version 1.0A
 * @date September 2024
 * @author Peter Antypas
 * @company Uluka Systems Pvt Ltd
 */

#ifndef CIRCULARQUEUE_HPP_
#define CIRCULARQUEUE_HPP_

#include <stdint.h>
#include <stddef.h>
#include "_assert.h"

/**
 * @brief A template-based circular queue class for buffering elements.
 * @tparam T The type of elements to be stored in the circular queue.
 * @details This class implements a circular queue with fixed-size buffer, allowing efficient storage and retrieval of elements in a FIFO manner.
 *          The buffer operates with a read and write pointer to manage the position of elements, ensuring efficient use of space.
 */
template<typename T>
class CircularQueue
{
public:
  /**
   * @brief Constructs a CircularQueue with the specified size.
   * @param size The size of the buffer to allocate for storing elements.
   * @details Initializes the read and write positions to 0 and allocates memory for the buffer.
   *          An assertion is used to ensure the buffer is valid in debug mode.
   */
  CircularQueue(size_t size)
  {
    mReadPosition = 0;
    mWritePosition = 0;
    mSize = size;
    mBuffer = new T[mSize];
    ASSERT_VALID_PTR(mBuffer);  // Ensure the buffer is valid
  }

  /**
   * @brief Checks if the circular queue is empty.
   * @return True if the queue is empty, false otherwise.
   */
  inline bool empty()
  {
    return mReadPosition == mWritePosition;
  }

  /**
   * @brief Pushes an element onto the circular queue.
   * @param Element The element to push onto the queue.
   * @return True if the element was successfully pushed, false if the queue is full.
   * @details If the next write position is safe (i.e., it doesn't overwrite the read position), the element is added to the buffer.
   */
  bool push(T &Element)
  {
    int nextElement = (mWritePosition + 1) % mSize;
    if ( isSafeToWrite(nextElement) )
    {
      mBuffer[mWritePosition] = Element;
      mWritePosition = nextElement;
      return true;
    }
    else
      return false;  // Queue is full
  }

  /**
   * @brief Pops an element from the circular queue.
   * @param Element A reference to store the element being popped.
   * @return True if an element was successfully popped, false if the queue is empty.
   * @details The element at the read position is returned, and the read position is advanced.
   */
  bool pop(T &Element)
  {
    if ( empty() )
      return false;

    Element = mBuffer[mReadPosition];
    mReadPosition = (mReadPosition + 1) % mSize;
    return true;
  }

private:
  /**
   * @brief Checks if it is safe to write to the next position.
   * @param nextElement The next write position in the buffer.
   * @return True if writing to the next position is safe, false otherwise.
   */
  inline bool isSafeToWrite(int nextElement)
  {
    return nextElement != mReadPosition;
  }

private:
  volatile int mReadPosition;    /**< Current read position in the circular buffer. */
  volatile int mWritePosition;   /**< Current write position in the circular buffer. */
  size_t mSize;                  /**< The size of the buffer. */
  volatile T* mBuffer = nullptr; /**< The buffer array for storing elements. */
};

#endif /* CIRCULARQUEUE_HPP_ */
