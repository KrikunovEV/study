					original
					
					Pos  Neg
					_________
				Pos	| TP  FP
predicted			|
				Neg	| FN  TN
				

Precision = TP / (TP + FP)
Recall = TP / (TP + FN)
=> Average precision

Intersection:
A - predicted
B - real
Ix0 = max (Ax0, Bx0)
Iy0 = max (Ay0, By0)
Ix1 = min (Ax1, Bx1)
Iy1 = min (Ay1, By1)

if Ix0 < Ix1 then there is intersection

IoU = S(intersection) / S(union)


for each class
	for match
		IoU > 0.5 -> TP++, FP++ other classes
		TP->confidence