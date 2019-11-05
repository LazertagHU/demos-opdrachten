.PHONY: clean All

All:
	@echo "----------Building project:[ OLED_DISPLAY - Debug ]----------"
	@cd "demos-opdrachten/OLED_CODE" && "$(MAKE)" -f  "OLED_DISPLAY.mk"
clean:
	@echo "----------Cleaning project:[ OLED_DISPLAY - Debug ]----------"
	@cd "demos-opdrachten/OLED_CODE" && "$(MAKE)" -f  "OLED_DISPLAY.mk" clean
