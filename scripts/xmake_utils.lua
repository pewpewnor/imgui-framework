function SuppressLsan()
	after_build(function(target)
		os.cp(
			path.join(os.projectdir(), "scripts", "lsan.supp"),
			path.join(path.directory(target:targetfile()), "lsan.supp")
		)
	end)
	add_runenvs("LSAN_OPTIONS", "suppressions=lsan.supp")
end
