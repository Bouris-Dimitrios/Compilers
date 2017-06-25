program completeTest
const RANGESTART := 5 , RANGEEND := 20;
var inputVar;

	function isInRange(num)
	begin
		if num >= 5 and num <= 20 then
			return (1)
		else
			return (0)
	end

	function countSum(start)
	var i,sum;
	begin
		sum := 0;
		for i := start to RANGEEND step 1
		begin
			print(i);
			sum := sum + i
		end;
		return (sum)
	end

	function countMul()
	var i,mul;
	begin
		mul := 1;
		for i := RANGESTART to RANGEEND step 5
		begin
			mul := mul * i
		end;
		return (mul)
	end

begin
	inputVar := 0 ;
	
	while inputVar <> -1 do
	begin
		input(inputVar);

		if isInRange(inputVar) = 1 then
		begin
			print(countSum(inputVar));
			print(1)
		end
		else
		begin
			print( countMul() ) ;
			print(-1)
		end
	end
end
