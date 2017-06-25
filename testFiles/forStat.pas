program forStat
const A := 10;
var  i,j,a;

function forTest1(var i ,var j)
begin
	for i := 0 to 10  step 2
		print(i+j);
	for j := 10 to 0 step -2 
		input (a);
	return (5)
end

procedure forTest2(var i ,var j)
begin

  print(a);

	for i:=1 to a step a
	begin 
		for j:=1 to 2*a step a
			print(i*j)
	end 
	
end
begin
  a:=forTest1(var i ,var j);
  print(a);
  call forTest2(var i , var j)

end


