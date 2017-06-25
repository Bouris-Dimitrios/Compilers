program selectTest
const i := 90;
var a;

procedure checkSelectIfPlease()
begin
	input(a);
	select if (a)
		is equal to: 1
		begin
			print(i-1)
		end
		is equal to : 2
		begin
			print(i*i)
		end
		is equal to: 3
		begin
			print(i+2)
		end 
		is equal to: 4
		begin
			print(0)
		end 
	endselect
end

begin

	call checkSelectIfPlease()
end
