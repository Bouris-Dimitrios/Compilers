program gcdProgram
var x,y,result ;





function gcd(a,b)
	function modOperator(a,b)
		var reminder;
		begin
			reminder := a - (a/b)*b;
			return (reminder)
			 	
		end
	begin
		if b = 0  then 
			return (a)
		else  
			return (gcd(b,modOperator(a,b)))	
	end

begin
	input(x);
	input(y);
	result := gcd(x,y);
	print(result)
end	
