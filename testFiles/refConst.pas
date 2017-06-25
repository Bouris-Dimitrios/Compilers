program refConst
const A := 256 ;
var a;

	procedure p1()
	const B := 128;
		procedure p2()
			const B := 64;
			procedure p3()
			begin
				print(B)	
			end
		begin
			print(A);
			call p3()
		end
	begin
		call p2()
	end



begin
	a := A * 2;
	print(a);
	call p1()
end
