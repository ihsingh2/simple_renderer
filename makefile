RENDER := ./build/render
SCENES := scenes
REPORT := report

all: q1 q3

q1: FORCE
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 1/directional_light.json $(REPORT)/q1/cornell_box_1.png 1 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 1/directional_light.json $(REPORT)/q1/cornell_box_32.png 32 0

q2: FORCE
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 2/scene1.json $(REPORT)/q2/scene1.png 32 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 2/scene2.json $(REPORT)/q2/scene2.png 32 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 2/scene3.json $(REPORT)/q2/scene3.png 32 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 2/scene4.json $(REPORT)/q2/scene4.png 32 0

q3: FORCE
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_0_10.png 10 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_0_100.png 100 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_0_1000.png 1000 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_1_10.png 10 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_1_100.png 100 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_1_1000.png 1000 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_2_10.png 10 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_2_100.png 100 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/small.json $(REPORT)/q3/small_2_1000.png 1000 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_0_10.png 10 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_0_100.png 100 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_0_1000.png 1000 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_1_10.png 10 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_1_100.png 100 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_1_1000.png 1000 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_2_10.png 10 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_2_100.png 100 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/med.json $(REPORT)/q3/med_2_1000.png 1000 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_0_10.png 10 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_0_100.png 100 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_0_1000.png 1000 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_1_10.png 10 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_1_100.png 100 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_1_1000.png 1000 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_2_10.png 10 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_2_100.png 100 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/big.json $(REPORT)/q3/big_2_1000.png 1000 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_0_10.png 10 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_0_100.png 100 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_0_1000.png 1000 0
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_1_10.png 10 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_1_100.png 100 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_1_1000.png 1000 1
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_2_10.png 10 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_2_100.png 100 2
	$(RENDER) $(SCENES)/Assignment\ 3/Question\ 3/many.json $(REPORT)/q3/many_2_1000.png 1000 2

FORCE:
