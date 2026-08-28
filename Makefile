.PHONY: clean All

All:
	@echo "==========Building project:[ 02-Command-Terminal-History-Tracker -  ]----------"
	@cd "02-Command-Terminal-History-Tracker" && "$(MAKE)" -f  "02-Command-Terminal-History-Tracker.mk"
clean:
	@echo "==========Cleaning project:[ 02-Command-Terminal-History-Tracker -  ]----------"
	@cd "02-Command-Terminal-History-Tracker" && "$(MAKE)" -f  "02-Command-Terminal-History-Tracker.mk" clean
