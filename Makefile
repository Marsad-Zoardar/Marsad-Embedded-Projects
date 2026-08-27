.PHONY: clean All

All:
	@echo "==========Building project:[ 01-Heap-Allocated-Circular-Buffer -  ]----------"
	@cd "01-Heap-Allocated-Circular-Buffer" && "$(MAKE)" -f  "01-Heap-Allocated-Circular-Buffer.mk"
clean:
	@echo "==========Cleaning project:[ 01-Heap-Allocated-Circular-Buffer -  ]----------"
	@cd "01-Heap-Allocated-Circular-Buffer" && "$(MAKE)" -f  "01-Heap-Allocated-Circular-Buffer.mk" clean
