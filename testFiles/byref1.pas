program byreftesting
var a,x;

procedure first(var a)
	var x;
	function second(var a)
								
		procedure third(a)
		begin 
			a:= -1;
			print(a)
			
		end
					
		begin
			a := a + 1;
			print(a);
			call third( a);
			return(1)
		end
begin
a := a + 1;
print(a);
x := second(var a)

end

begin
	input(a);
	call first(var a);
	print(a)

end

