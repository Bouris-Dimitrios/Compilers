program pascv
var h;
	function yourFunc(e)
		procedure yourFuncProcedure()
			begin
				e := e+2/2;
				print(e)					
			end
		begin
			e := e*3;
			print(e);
			call yourFuncProcedure();
			return(e)
		end

	procedure myProc(c)
		procedure myProcProcedure(var y)
			begin
				y := 0;
				print(y)
			end
	begin
		c:=c/2;
		print(c);
		c := yourFunc(c);
		print(c);
		call myProcProcedure(var c);
		print(c)
	end
begin
	h:=2000;
	call myProc(h);
	print(h)
end
