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
			call third( a);
			return(1)
		end
begin
a := a + 1;
print(a);
x := second(var a)
end

function p(var a)
	
	procedure p2()
		begin
			print(x);
			a:= 1;
			print(a)
		end
	begin
                call p2();
		return(1)
	end


function c(b)

	procedure p3()
		begin
			b:= 1;
			print(b)
		end
	begin
                call p3();
		return(1)
	end


begin

    input(a);
    call first(var a);
    print(a);


    x := 1234;
    input(a);
    call p(var a);
    print(a);


    input(a);
    x := c(a);
    print(a)

    
end
