program finalTest
const Pi:=3,e:=2;
var myVar;


function max(a,b)
begin
	if a > b  then
		return (a)
	else if a < b then
		return (b)
	else
		return (a)

end

function min(a,b)
begin
	if a > b  then
		return (b)
	else if a < b then
		return (a)
	else
		return (b)

end
begin

myVar := max(max(5,3),(min(2,8)));
print(myVar)

end
