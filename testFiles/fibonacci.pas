program fibonacci
var n;
var apotelesma;


function f(n)
begin
	if n = 0 or n = 1 then
		return(n)
	else
		return(f(n-1)+f(n-2))	
end 

begin
n := 0;
while  n >= 0 do
	begin 

		input(n);

		if n >= 0 then
		begin
			apotelesma := f(n);
			print(apotelesma)
		end

	end
end
