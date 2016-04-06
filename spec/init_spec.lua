describe("main library entry point", function()
	local spawn = require "spawn"
	it("can run `true`", function()
		assert.same({true, "exit", 0}, {spawn.run("true")})
	end)
	it("can run `false`", function()
		assert.same({nil, "exit", 1}, {spawn.run("false")})
	end)
	it("returns code 127 when command doesn't exist", function()
		assert.same({nil, "exit", 127}, {spawn.run("made-up-command")})
	end)
	it("emulates system() by running a shell", function()
		assert.same({true, "exit", 0}, {spawn.system("true")})
		assert.same({nil, "exit", 2}, {spawn.system("exit 2")})
	end)
	it("returns exit signal correctly", function()
		-- 15 is SIGTERM
		assert.same({nil, "signal", 15}, {spawn.system("kill $$")})
		-- 9 is SIGKILL
		assert.same({nil, "signal", 9}, {spawn.system("kill -9 $$")})
	end)
end)