#pragma once

enum		CmdType
  {
    REPLY_CODE,
    HANDSHAKE_SYN,
    HANDSHAKE_SYN_ACK,
    HANDSHAKE_ACK,
    GET_ROOM_LIST,
    CREATE_ROOM,
    LEAVE_ROOM,
    JOIN_ROOM,
    GET_ROOM,
    SET_STATUS,
    RENAME_ROOM,
    ALIVE,
    UPDATE_ROOM,
    LAUNCH_GAME,
    END_GAME,
    EMPTY_TYPE
  };

enum		CmdName
  {
    CHAT_INFO,
    ROOM_INFO,
    ROOM_LIST,
    ENTITY,
    INPUT_CMD,
    BASIC_CMD,
    EMPTY_NAME
  };
