local id1=
local id2=
local id3=

local allowed_users_id = {id1,id2,id3}

SCRIPTS_FOLDER="/home/wlanp/presentacion/telegram/"

local path = '/home/wlanp/presentacion/telegram/toSend/packet.txt'
local open = io.open

local function read_file(path)
	print(path)
	local file = open(path, "rb") -- r read mode and b binary mode
    	if not file then return nil end
    	local content = file:read "*a" -- *a or *all reads the whole file
    	file:close()
    	return content
end

function on_msg_receive (msg)
	if msg.out then
		return
	end
	for _,v in pairs(allowed_users_id) do
		print(v)
		if(v == msg.from.peer_id ) then
			if (msg.text=='ping') then
				os.execute('/home/wlanp/presentacion/telegram/read_packets')
                        	local fileContent = read_file(path)
                        	print (fileContent)
				send_msg (msg.from.id, fileContent, ok_cb, false)
			end
  		end
   	end
	end

  function on_our_id (id)
  end
   
  function on_secret_chat_created (peer)
  end
   
  function on_user_update (user)
  end
   
  function on_chat_update (user)
  end
   
  function on_get_difference_end ()
  end
   
  function on_binlog_replay_end ()
  end
