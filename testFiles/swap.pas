program swap
var num1,num2;

procedure switch(var a,var b)
	var temp ;
	begin
		temp := a;
		a := b;
		b := temp
	end

procedure switch2( a, b)
	var temp ;
	begin
		temp := a;
		a := b;
		b := temp
	end


begin 

	input(num1);
	input(num2);
	call switch(var num1,var num2);
	print(num1);
	print(num2);



	input(num1);
	input(num2);
	call switch2( num1, num2);
	print(num1);
	print(num2)
end
