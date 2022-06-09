NAME=dokumentace

all:
#### by pdflatex (2x beacause of cross-referencing) without bibtex
	pdflatex $(NAME).tex
	bibtex $(NAME)
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex
#### by latex, dvips, ps2pdf
# 	latex $(NAME).tex
# 	bibtex $(NAME)
# 	latex $(NAME).tex
# 	latex $(NAME).tex
# 	dvips -t a4 $(NAME).dvi
# 	ps2pdf $(NAME).ps
clean:
#### by pdflatex
	rm $(NAME).aux $(NAME).log $(NAME).pdf $(NAME).bbl $(NAME).blg $(NAME).toc $(NAME).out
### by latex, dvips, ps2pdf	
# 	rm $(NAME).aux $(NAME).dvi $(NAME).log $(NAME).pdf $(NAME).ps $(NAME).bbl $(NAME).blg