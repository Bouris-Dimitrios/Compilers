program ifSelectif
var x;
	procedure ifCheck(tax)
		begin
			if x < 100  then
				tax := 101
			else if  x >= 100 and x < 200  then
				tax := 151
			else if  x >= 200  then
				tax := 201;
			print(tax)
		end
begin
	
	input (x);
	call  ifCheck(x);
	print(x)
end
