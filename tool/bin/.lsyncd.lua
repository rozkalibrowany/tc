settings {
	nodaemon = true,
	insist = true,
}

local function ends_with(str, ending)
	return ending == "" or str:sub(-#ending) == ending
end

hsts = {}
exclude=os.getenv("WsLsyncExclude")
source=os.getenv("WsLsyncSource")
target=os.getenv("WsLsyncTarget")
if exclude == nil then
	hsts = {}
else
	hosts=os.getenv("WsLsyncHosts")
	if hosts ~= nil then
		hsts = {}
		for host in string.gmatch(hosts, '([^, ]+)') do
			table.insert(hsts, host)
			print(host)
		end
	end
end

for k,v in pairs(hsts) do
	print (k, v)
end

tgts = {
 	['ws'] = {
		'git'
	},
}

function isstring(tgt)
	return type(tgt) == "string"
end

function _exclude(tgt, exclude)
	if ends_with(tgt, 'third-party-libs') then
		return nil
	end
	return exclude
end

function _sync(hst, src, tgt, exclude)
	exclude_from = _exclude(src, exclude)
	print ('tgt', src .. '  ->  '.. hst ..':' .. tgt)
	if true then
		sync {
			default.rsyncssh,
			source = src,
			targetdir = tgt,
			delay = 2,
			delete = true,
			excludeFrom = exclude_from,
			host = hst,
			rsync = {
				rsync_path = 'mkdir -p ' .. tgt .. ' && rsync',
				archive  = true,
				compress = true,
				verbose = true,
				owner = false,
				group = false,
			}
		}
	end
end


function _ssync(hst, tgt, exclude)
	print (tgt, exclude_from)
end

for _, hst in ipairs(hsts) do
	T = tgts[hst]
	if T == nil then
		T = tgts['any']
	end
	for d0, d1 in pairs(T) do


		if isstring(d0) then
			for _, d2 in ipairs(d1) do
				src = source .. '/' .. d0 .. '/' .. d2
				tgt = target .. '/' .. d0 .. '/' .. d2
				_sync(hst, src, tgt, exclude)
			end
		else
			src = source .. '/' .. d1
			tgt = target .. '/' .. d1
			_sync(hst, src, tgt, exclude)
		end
	end
end
