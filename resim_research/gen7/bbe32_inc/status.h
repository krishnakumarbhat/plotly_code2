/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef STATUS_H
   #define STATUS_H

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Status return codes.
 * Common error codes will be a unified enumeration (C enum) that will contain all error codes
 * (common and specific). There will be separate "error values spaces" (or slots), each of 256
 * positions, allocated per functionality.
 */
typedef enum
{
   /* Generic error codes */
   STATUS_SUCCESS     = 0x000U, /*!< Generic operation success status */
   STATUS_ERROR       = 0x001U, /*!< Generic operation failure status */
   STATUS_BUSY        = 0x002U, /*!< Generic operation busy status */
   STATUS_TIMEOUT     = 0x003U, /*!< Generic operation timeout status */
   STATUS_UNSUPPORTED = 0x004U, /*!< Generic operation unsupported status */
   STATUS_EXCEPTION   = 0x005U, /*!< Generic exception occurred status */
} status_t;

#endif /* STATUS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
