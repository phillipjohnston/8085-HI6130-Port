START: 	NOP
		MVI A,27H
		STA 800H
		STA 7F00H
		
		MVI A,0H
		LDA 7F01H
		STA 801H
		STA 7F00H
		
		MVI A,0AAH
		STA 7F02H
ETERNAL: 	NOP
			JMP START