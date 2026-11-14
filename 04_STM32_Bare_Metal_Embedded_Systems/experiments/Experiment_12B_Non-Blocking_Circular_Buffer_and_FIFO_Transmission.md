Experiment 12B: Circular FIFO and Nonblocking UART Transmission

Project: 04 — STM32 Bare-Metal Embedded Systems

Microcontroller: STM32F446RE

Peripheral: USART2

Experiment status: Completed — Hardware Verified

1. Objective

The objective of this experiment was to implement a circular First-In, First-Out (FIFO) transmit buffer for nonblocking USART2 communication.

In Experiment 12A, we implemented basic USART2 transmission using direct register manipulation. However, the USART hardware may not always be ready to accept a new character immediately.

Instead of blocking the CPU while waiting for the USART transmit data register to become available, we designed a 16-byte circular buffer to temporarily hold outgoing characters.

The system should:

Enqueue characters in FIFO order.

Reject new characters when the buffer is full without overwriting existing data.

Support wraparound of the head and tail indices.

Allow the oldest character to be examined without removing it.

Transfer a queued character to USART2 only when the hardware can accept it.

Return control to the cooperative scheduler without waiting for transmission to complete.

This design supports continued execution of other scheduled tasks while UART transmission is pending.

2. Hardware and Software Setup

Component | Configuration
-----------:|-----------------
Microcontroller | STM32F446RE
Development board | STM32 NUCLEO-F446RE
UART peripheral | USART2
TX pin | PA2, Alternate Function 7
Baud rate | 115200
Frame configuration | 8 data bits, no parity, 1 stop bit (8N1)
Serial terminal | PuTTY via ST-LINK Virtual COM Port
Development environment | STM32CubeIDE
Programming method | Bare-metal C, direct register access
Compiler optimization | Initially -O2; changed to -O0 for debugging
FIFO capacity | 16 bytes


Register interactions

The following registers were configured or accessed by the USART2 driver established in Experiment 12A:

RCC_AHB1ENR — Enable GPIOA peripheral clock.

RCC_APB1ENR — Enable USART2 peripheral clock.

GPIOA_MODER — Configure PA2 for alternate function mode.

GPIOA_AFRL — Select AF7 for USART2 TX.

USART2_BRR — Configure the baud rate.

USART2_CR1 — Enable USART2 transmission and peripheral operation.

USART2_SR — Inspect the TXE flag.

USART2_DR — Write outgoing characters to the transmit data register.

3. Implementation

3.1 Circular FIFO data structure

The transmit buffer was implemented as a structure containing a 16-byte array and three management variables.

#define UART_TX_BUFFER_SIZE 16u
```C
typedef struct {
    uint8_t data[UART_TX_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} UartTxBuffer;
```
The variables serve the following purposes:

head: Index where the next character will be inserted.

tail: Index of the oldest queued character.

count: Number of characters currently stored.

The head and tail indices wrap around using modulo arithmetic:

nextIndex = (currentIndex + 1u) % UART_TX_BUFFER_SIZE

The count variable distinguishes a full FIFO from an empty FIFO even when head == tail.

3.2 FIFO functions

UART_TX_Push()

Attempts to insert a character into the circular buffer. Returns true when insertion succeeds and false when the buffer is full.

UART_TX_Pop()

Removes the oldest queued character, stores it through an output pointer, advances the tail index, and decreases the stored-character count.

UART_TX_Peek()

Reads the oldest queued character without modifying the FIFO indices or count. Returns false if the buffer is empty or an input pointer is invalid.

USART2_tryTransmit()

Checks the USART2 TXE flag. If TXE is set, the function writes one character to USART2_DR and returns true. Otherwise, it returns false without blocking.

UART_TX_Service()

Connects the FIFO to the hardware transmitter.
```C
static void UART_TX_Service(UartTxBuffer *buffer)
{
    uint8_t peek = 0;

    if (!UART_TX_Peek(buffer, &peek)) {
        return;
    }

    if (!USART2_tryTransmit((char)peek)) {
        return;
    }

    (void)UART_TX_Pop(buffer, &peek);
}
```
The service function performs three operations:

Peek at the oldest character.

Attempt transmission when USART2 is ready.

Remove the character only after USART2 accepts it.

If the buffer is empty or the USART hardware is not ready, the service returns immediately.

The function is called during each iteration of the cooperative scheduler:
```C
for (;;) {
    // Sensor, communication, and LED tasks

    UART_TX_Service(&txBuffer);
}
```
4. Test Methodology and Results

The experiment was divided into four tests. Each test was executed with an appropriate initialized buffer state to isolate the behavior under investigation.

Test 1 — FIFO Insertion

Objective: Verify sequential insertion and FIFO index updates.

Three characters, A, B, and C, were inserted into an initially empty buffer.
```C
UartTxBuffer txBuffer = {0};

UART_TX_Push(&txBuffer, 'A');
UART_TX_Push(&txBuffer, 'B');
UART_TX_Push(&txBuffer, 'C');
```

Variable | Expected | Observed
|---|---:|---:|
head | 3 | 3
tail | 0 | 0
count | 3 | 3

Result: Passed.

The head advanced after every insertion, while the tail remained unchanged. The count increased from zero to three.

Evidence: Experiment_12B_FIFO_insertion.png

Test 2 — Overflow Protection

Objective: Verify that the buffer rejects insertion when all 16 slots are occupied.

The buffer was filled with characters A through P. A 17th insertion was then attempted.
```C
for (uint8_t i = 0; i < UART_TX_BUFFER_SIZE; i++) {
    UART_TX_Push(&txBuffer, (uint8_t)('A' + i));
}

bool overflowResult = UART_TX_Push(&txBuffer, 'Q');
```
Variable | Expected | Observed
|----:|----:|----:|
head | 0 | 0
tail | 0 | 0
count | 16 | 16
overflowResult | false | false

Result: Passed.

After 16 successful insertions, the head wrapped to index zero. The additional insertion was rejected, and the queue remained full without overwriting stored characters.

Evidence: Experiment_12B_Test_2_Overflow.png

Test 3 — Circular Buffer Wraparound

Objective: Verify correct FIFO behavior after the indices wrap around.

Starting from the full buffer in Test 2, three characters (A, B, and C) were removed. Three additional characters (Q, R, and S) were then inserted.

```C
uint8_t poppedCharacter = 0u;

for (uint8_t j = 0; j < 3; j++) {
    UART_TX_Pop(&txBuffer, &poppedCharacter);
}

for (uint8_t k = 0; k < 3; k++) {
    UART_TX_Push(&txBuffer, (uint8_t)('Q' + k));
}
```
Operation | Initially full | After three pops | After three pushes
|----:|----:|----:|----:|
Head | 0 | 0 | 3
Tail | 0 | 3 | 3
Count | 16 | 13 | 16


The UART_TX_Peek() function was used to inspect the oldest remaining character.

Observed peek result: D (ASCII 68).

Result: Passed.

The test confirmed that head and tail wraparound maintained FIFO ordering. It also demonstrated that equal head and tail indices can represent a full buffer when count == 16.

Evidence: Experiment_12B_Test_3_1_Pop_Char.png
Experiment_12B_Test_3_2_Push_Char.png

Test 4 — Nonblocking UART Hardware Transmission

Objective: Verify that queued characters are transmitted to USART2 in the same order in which they were inserted.

A fresh empty buffer was initialized with A, B, and C. The cooperative scheduler repeatedly called UART_TX_Service().

Each successful service operation removed one queued character.

Successful service call | Head | Tail | Count | Character transmitted
|------------:|---------:|---------:|---------:|-------:|
Initial state | 3 | 0 | 3 | —
First | 3 | 1 | 2 | A
Second | 3 | 2 | 1 | B
Third | 3 | 3 | 0 | C

The serial terminal displayed:

ABC

Result: Passed.

The application successfully transferred all queued characters to USART2 in FIFO order. After transmission, the buffer returned to an empty state.

Evidence: Experiment_12B_Test_4_PuTTY_Output.png
Experiment_12B_Test_4_PuTTY_Config.png

5. Analysis

5.1 FIFO integrity

The insertion, removal, overflow, and wraparound tests demonstrated that the circular buffer maintained correct FIFO behavior under the tested conditions.

The buffer did not overwrite previously queued characters when full. Instead, it rejected additional insertions.

The wraparound test confirmed that the logical ordering of characters was preserved even when the physical storage indices returned to the beginning of the array.

5.2 Nonblocking communication

The principal improvement over a blocking UART implementation was the separation of data generation from hardware transmission.

Application code can enqueue a character without waiting for the USART peripheral to complete a transmission.

The service function checks the TXE flag before writing to the hardware data register. If USART2 cannot accept a character, the function leaves that character queued and returns immediately.

This behavior is particularly useful in a cooperative scheduler, where long blocking operations can delay other tasks.

5.3 Memory efficiency

The FIFO uses statically allocated storage.

It does not require malloc(), realloc(), or other dynamic allocation operations.

Its storage requirement is fixed and predictable, which is desirable for embedded systems with limited RAM.

5.4 Buffer limitations

The FIFO has a fixed capacity of 16 bytes.

If the application generates characters faster than USART2 can accept them for a sustained period, the buffer may become full. In this situation, new enqueue attempts fail.

The FIFO does not guarantee lossless communication under every operating condition. Applications must handle failed enqueue attempts according to their reliability requirements.

Furthermore, successful removal from the software queue indicates that USART2 accepted the byte into its transmit data register. It does not guarantee that a receiving device successfully received the character.

6. Debugging Challenges and Lessons Learned

6.1 Compiler optimization and breakpoints

The initial debugging sessions used GCC optimization level -O2.

During source-level debugging, a breakpoint intended for UART_TX_Push() did not consistently produce the expected stopping location. In one instance, the debugger stopped inside the dwtInit() function.

The compiler optimization level was changed to -O0, followed by a clean rebuild and a new debugging session.

With optimization disabled, source-level stepping and variable inspection behaved as expected.

Lesson learned: Compiler optimization can make source-level debugging more difficult by changing instruction ordering, inlining functions, and modifying source-to-machine-code mappings.

6.2 Unexpected UART output

During testing, the serial terminal displayed ABC before the expected transmission sequence.

Further investigation revealed leftover UART test statements from earlier experiments.

These statements were removed or disabled, allowing the current test to be observed independently.

Lesson learned: Previous experimental code can interfere with subsequent tests. Experimental configurations should be isolated and carefully controlled.

6.3 Loop-variable corruption

During the FIFO pop test, a loop counter was initially used as the destination for a popped character.

Because UART_TX_Pop() writes the removed character through the provided pointer, the loop counter was overwritten by the character's ASCII value.

The issue was corrected by introducing a separate uint8_t variable to hold popped characters.

Lesson learned: Functions that modify data through pointers must be used carefully. Passing a loop-control variable as an output parameter can unintentionally change program flow.

7. Lessons Learned

This experiment strengthened understanding of:

Circular buffer data structures and FIFO ordering.

Pointer-based function interfaces.

Buffer overflow protection.

Modulo arithmetic for index wraparound.

Nonblocking peripheral access using hardware status flags.

Separation of software buffering from hardware transmission.

Direct register-level USART2 programming.

Source-level debugging and compiler optimization.

Experimental verification using debugger variables and real hardware output.

8. Limitations and Future Work

Although all four planned tests passed, additional validation is needed before using the implementation as a general-purpose UART driver.

The current implementation:

Uses polling rather than interrupts or DMA.

Has a fixed 16-byte transmit buffer.

Has not been validated for concurrent access from interrupt handlers and the main loop.

Has not been subjected to exhaustive timing or stress testing.

Does not provide an end-to-end delivery acknowledgment or recovery protocol.

Future experiments will introduce periodic communication, dropped-byte monitoring, multi-character string transmission, and more advanced UART driver architectures.

9. Conclusion

Experiment 12B successfully demonstrated the implementation and hardware verification of a 16-byte circular FIFO for nonblocking USART2 transmission on the STM32F446RE.

All four planned tests passed, including FIFO insertion, overflow protection, index wraparound, and hardware transmission.

The resulting system supports buffering outgoing characters while the USART hardware is temporarily unavailable, allowing the cooperative scheduler to continue executing other tasks.

This experiment establishes the foundation for future work on periodic UART communication, diagnostic telemetry, and interrupt-driven transmission.

Final status: PASSED — Planned tests verified on hardware.