Experiment 12C — Periodic UART Producer and FIFO Overflow

1. Objective

Integrate our Experiment 12B circular UART FIFO into the existing cooperative scheduler.

In Experiment 12B, we manually inserted characters into the buffer and transmitted them.

In Experiment 12C, the scheduler becomes responsible for producing UART data periodically, while a separate service function handles transmission.

We want to investigate three things:

Whether a periodic task can enqueue data without blocking the processor.

Whether UART transmission can occur independently of the producing task.

What happens when the producer generates data but the UART service is disabled.

2. Hypothesis

The communication task is expected to attempt to enqueue one character, 'A', into the circular UART buffer every 200 ms using UART_TX_Push(). When UART_TX_Service() executes independently within the main scheduler loop, the queued characters should be transmitted through USART2 and displayed continuously on PuTTY.

During normal operation, the FIFO's head and tail indices are expected to advance after successful insertion and removal operations, respectively, wrapping from index 15 back to index 0. Because the transmission service operates much more frequently than the 200 ms producer task, the FIFO occupancy (count) is expected to remain near zero, and uartDroppedBytes should remain zero.

In Test 2, when UART_TX_Service() is disabled while the periodic producer remains active, the FIFO is expected to fill after 16 successful insertions. At this point, the expected buffer state is head = 0, tail = 0, and count = 16. Subsequent calls to UART_TX_Push() should return false, and uartDroppedBytes should increase by one for each rejected enqueue attempt, occurring approximately every 200 ms.



3. Hardware-Software Setup

Component | Configuration
|----|----|
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
Communication task period | 200 ms.


4. Implementation & Expected Behavior

Test 1 — Normal UART operation

Enable the periodic producer and the UART service.

Expected behavior:

Every 200 ms, the communication task attempts to enqueue 'A'.

UART_TX_Service() gradually drains the FIFO.

PuTTY displays a continuous sequence of A characters.

The FIFO should normally remain nearly empty.

uartDroppedBytes should remain zero under normal conditions.

Use USART2, 115200 baud, with your previously verified serial configuration.

Test 2 — FIFO overflow with UART servicing disabled

Keep the producer enabled, but disable the UART service.

Expected behavior:

The first 16 enqueue attempts succeed.

The buffer becomes full.

Subsequent attempts fail.

uartDroppedBytes increases by one for each rejected insertion.



5. Test Procedure

Evidence: Here are the results from our tests

Test 1:
```C
253 #if EXPERIMENT_12C_TEST1

254	UartTxBuffer txBuffer = {0};

255	volatile uint32_t uartDroppedBytes = 0u;

256 #endif
```

```C
435 #if EXPERIMENT_12C_TEST1

436

437				if(!UART_TX_Push(&txBuffer, 'A')){

438					uartDroppedBytes++;

439				}

440 #endif
```
```C
490 #if EXPERIMENT_12C_TEST1

491		//Transmit the enqueued character 'A'

492		UART_TX_Service(&txBuffer);

493 #endif
```




Test 2:
```C
253 #if EXPERIMENT_12C_TEST2

254	UartTxBuffer txBuffer = {0};

255	volatile uint32_t uartDroppedBytes = 0u;

256 #endif
```

```C
435 #if EXPERIMENT_12C_TEST2

436

437				if(!UART_TX_Push(&txBuffer, 'A')){

438					uartDroppedBytes++;

439				}

440 #endif
```

6. Results

Test 1 — Normal UART Operation

Test 1 initializes the FIFO, enables the periodic producer and UART service, then captures PuTTY and debugger results. Test 2 starts with an empty FIFO, enables the producer, disables the service, and captures the overflow state. The program was executed for approximately 43 seconds before the debugger values were inspected. The PuTTY terminal displayed a continuous sequence of 'A' characters, confirming successful transmission through USART2.

The debugger reported the following FIFO state:

Variable | Observed Value

|----|----|

txBuffer.head | 8

txBuffer.tail | 8

txBuffer.count | 0

uartDroppedBytes | 0

The head index identifies the next insertion position, while the tail index identifies the next character to be removed from the FIFO. Both indices advance after successful operations and wrap around when they reach the end of the 16-byte buffer.

The matching head and tail values, together with count = 0, indicate that the FIFO was empty when the debugger captured the results.

The continuous UART output and absence of dropped bytes indicate that the UART service was able to keep up with the periodic producer during the observed test.

Test 2 — FIFO Overflow Protection

The UART service was disabled while the communication task continued attempting to enqueue one 'A' character every 200 ms.

The program was observed at currentTime = 4267 ms. Assuming the first communication-task release occurred at 200 ms and subsequent releases occurred at the specified interval, the expected number of enqueue attempts was:

Floor (4267/200) = 21

Because the FIFO capacity is 16 bytes, the first 16 insertions were expected to succeed. The remaining attempts should fail:

21 - 16 = 5

The debugger reported:

Variable | Observed Value

|----|----|

txBuffer.head | 0

txBuffer.tail | 0

txBuffer.count | 16

uartDroppedBytes | 5

These observations match the predicted behavior.

After 16 successful insertions, the head index wrapped around zero. Because UART servicing was disabled, no characters were removed, so the tail index remained at zero.

The buffer reached its maximum capacity of 16 characters. Each subsequent failed call to UART_TX_Push() incremented uartDroppedBytes.

The results demonstrate that the FIFO correctly rejects additional data when full and records the number of rejected insertions without overwriting queued characters.

7. Technical Analysis

Experiment 12C demonstrates the importance of separating data production from hardware transmission in a cooperative embedded scheduler.

The communication task attempts to enqueue one character every 200 ms, while UART_TX_Service() independently checks whether USART2 is ready to accept the next queued character. This allows the producer to continue operating without waiting for UART transmission to complete.

Under normal operating conditions, the UART service was able to process the queued characters without any recorded data loss. The FIFO remained empty at the observed debugger checkpoint, indicating that the service was able to keep pace with the periodic producer during the test.

When UART servicing was disabled, the buffer occupancy increased until it reached its maximum capacity of 16 bytes. Since no characters were removed, subsequent insertion attempts were rejected. The uartDroppedBytes counter correctly recorded these failures, preventing queued data from being silently overwritten.

This behavior highlights an important limitation of finite buffering. Increasing FIFO capacity can delay saturation, but it cannot prevent overflow indefinitely if the data production rate continuously exceeds the consumption rate.

For example, increasing the FIFO capacity from 16 to 64 bytes would increase the time required to fill an initially empty buffer from approximately 3.2 seconds to 12.8 seconds, assuming one enqueue attempt every 200 ms and no servicing.

From a real-time systems perspective, the experiment illustrates that nonblocking communication requires both sufficient buffer capacity and an appropriate servicing strategy. The producer and consumer must operate at compatible rates to maintain reliable data transmission.

Although this experiment verifies FIFO saturation behavior and dropped-byte accounting, further measurements would be required to determine how UART servicing affects CPU utilization, scheduler jitter, and task deadline compliance.

8. Limitations

The current experiment used a fixed communication-task period of 200 ms, single-byte messages, and one UART interface. Therefore, the results are limited to this specific operating configuration.

The tests did not directly measure CPU utilization, scheduling jitter, worst-case UART service latency, or the maximum FIFO occupancy reached during normal operation. Although FIFO saturation was demonstrated with UART servicing disabled, the experiment did not investigate variable production rates, burst transmissions, or temporary delays in UART servicing.

Future experiments could evaluate different producer periods, message lengths, buffer capacities, and UART transmission strategies to determine their effects on data loss, scheduling performance, and communication reliability.

9. Conclusion

Experiment 12C successfully demonstrated the integration of a periodic UART producer and a nonblocking transmission service into a cooperative scheduler.

During normal operation, the communication task attempted to enqueue one character every 200 ms, while the independent UART service transmitted queued data through USART2. The PuTTY output confirmed successful communication, and the debugger recorded an empty FIFO with zero dropped bytes at the observation point.

When UART servicing was disabled, the 16-byte FIFO reached its maximum capacity. Subsequent enqueue attempts were rejected, and the dropped-byte counter accurately recorded five failed insertions during the observed test period.

The experiment demonstrated that separating data production from hardware transmission allows the communication task to operate without waiting for UART hardware readiness. However, reliable communication still depends on sufficient buffer capacity and an appropriate servicing strategy. Future investigations should evaluate communication throughput, scheduling latency, and FIFO behavior under varying workloads.