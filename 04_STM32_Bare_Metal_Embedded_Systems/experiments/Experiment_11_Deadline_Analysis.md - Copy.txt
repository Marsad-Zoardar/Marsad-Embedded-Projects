Project 04 – Bare-Metal GPIO Programming 

Experiment 11:  Deadline monitoring under scheduling interference 

Objective: We have developed a cooperative scheduler running on STM32 NUCLEO-F446RE  

Our scheduler currently handles three tasks: 

Task 

Period 

Behavior 

LED 

97 ms 

Toggle PA5 

Communication 

200 ms 

Simulated Communication Operation 

Sensor 

100 ms 

Nonblocking sensor conversion lasting 10 ms 

We have already measured scheduling latency, sensor completion latency, CPU execution cycles, and end-to-end response time. 

However, we have not yet demonstrated that the scheduler can evaluate deadline-miss detection under controlled scheduling interference. 

A task might be executed correctly and still produce its result too late to be useful. That's what we're investigating. 

Experiment configuration 

Parameter 

Value 

MCU 

STM32-F446RE 

CPU Clock 

16 MHz 

Scheduler 

Cooperative, non preemptive 

Sensor Period 

100 ms 

Sensor Conversion Duration 

10 ms 

Sensor Relative Deadline 

10 ms 

Timing Reference 

SysTick, 1 Ms 

Execution Cycle Measurement 

DWT_CYCCNT 

Compiler Optimization 

-O2 

 

For now, we will use only the original sensor_update() implementation keeping the inline/non-inline comparison disabled. 

Hypothesis: Our prediction is that, under normal operation, the sensor task will not encounter any significant latency to miss its deadline. But, when we introduce simulated delay, we will detect start latencies, completion latencies, and deadline misses. Firmware instrumentation records scheduling latency, completion latency, response time, and deadline violations. Suppose; 

Relative Deadline D = 10 ms 

Response Time = R 

Scheduled start time = Lscheduled 

Actual Completion Time = Lcomplete 

Then our intended deadline misses will be, 

R = Lcomplete - Lscheduled  

If, R > D, then we have a missed deadline. Our variable meauredDeadlineMissCount will be incremented. 

Implementation: We are dividing our experiment in two parts.  

Test A - Normal operation: In test A, we will run the program in its usual configuration. The LED, Communication, and Sensor task operated without any artificial delay. 

Test B – Scheduling interference: We will use our 10 ms dwt_delay to inside the LED task and record the latencies and delays.  

Evidence:  

Test A: We ran test A for approximately 30.3 seconds, and the results are given below: 

Variables 

Value 

reportedCompletedCount 

303 

reportedMaxResponseTimeMs 

10 

measuredDeadlineMissCount 

0 

maxStartLatencyMs 

0 

maxCompletionLatencyMs 

0 

It is evident that our prediction matched the output we see. In a cooperative, non-preemptive program, there were no deadline misses.  

 

Test B: We ran test B approximately 29.4 ms and here is the output: 

Variables 

Value 

reportedCompletedCount 

294 

reportedMaxResponseTimeMs 

19 

measuredDeadlineMissCount 

57 

maxStartLatencyMs 

9 

maxCompletionLatencyMs 

9 

When we introduced a 10 ms delay to the toggle LED program, it forced our sensor task to miss some deadlines. Deadline miss rate; 

Miss Rate = (Number of missed deadline/Total count) X 100 

=> (57/294) x 100 = 19.38% 

Program:
		uint32_t sensorStartCycles_original = DWT_CYCCNT;
		sensor_update(
				milliseconds,
				&sensorConfig,
				&sensorRuntime_original,
				&sensorStats_original
		);
		uint32_t sensorEndCycles_original = DWT_CYCCNT;
		uint32_t sensorExecutionCycles_original = sensorEndCycles_original - sensorStartCycles_original;
		measuredCycles_original = sensorExecutionCycles_original;
			if(sensorExecutionCycles_original < sensorStats_original.minExecutionCycles_original){
				sensorStats_original.minExecutionCycles_original = sensorExecutionCycles_original;
			}
			if(sensorExecutionCycles_original > sensorStats_original.maxExecutionCycles_original){
				sensorStats_original.maxExecutionCycles_original = sensorExecutionCycles_original;
			}
			sensorStats_original.executionSamples_original++;

			//EXPERIMENT 11
			reportedCompletedCount = sensorStats_original.completedCount_original;
			reportedMaxResponseTimeMs = sensorStats_original.maxResponseTimeMs_original;
			measuredDeadlineMissCount = sensorStats_original.deadlineMissCount_original;
			maxStartLatencyMs = sensorStats_original.maxStartLatencyMs_original;
			maxCompletionLatencyMs = sensorStats_original.maxCompletionLatencyMs_original;
 

Conclusion:  

Experiment 11 demonstrated the effect of blocking operations on deadline compliance in a cooperative, non-preemptive scheduler running on the STM32F446RE. 

Under normal operation (Test A), the sensor completed 303 conversions without any recorded deadline misses. The maximum observed response time was 10 ms, equal to the configured relative deadline. No additional start or completion latency was detected at the 1 ms measurement resolution. 

After introducing a 10 ms blocking delay into the LED task (Test B), the sensor experienced increased scheduling interference. The maximum observed response time increased to 19 ms, while both maximum start latency and maximum completion latency reached 9 ms. Of 294 completed conversions, 57 missed their deadlines, corresponding to a deadline-miss rate of approximately 19.39%. 

These results support the hypothesis that blocking operations can reduce the timing responsiveness of a cooperative scheduler and cause deadline violations in other tasks. The experiment also demonstrated that the implemented deadline-monitoring mechanism recorded these violations under the tested conditions. 

However, the measurements are limited by the 1 ms SysTick resolution, the simulated sensor workload, and the tested execution conditions. The observed maximum response time should not be interpreted as a guaranteed worst-case response time. 

 

 

 