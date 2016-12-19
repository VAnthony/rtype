#include	"Serialize.hh"

Serialize::Serialize()
{

}

Serialize::~Serialize()
{

}

char		*Serialize::serialize(IEntity *entity)
{
  packet	p;
  std::string	tmps;
  int		i;
  char		*ret;
  int		j;
  double	tmpd;
  int		tmpi;

  ret = new char[65471];
  p.dataType = entity->getType();
  p.cmdType = ENTITY;
  j = 0;
  tmpd = entity->getPosX();
  i = -1;
  while (++i != 8)
    p.data[j++] = reinterpret_cast<char*>(&(tmpd))[i];
  tmpd = entity->getPosY();
  i = -1;
  while (++i != 8)
    p.data[j++] = reinterpret_cast<char*>(&(tmpd))[i];
  tmpd = entity->getSpeedX();
  i = -1;
  while (++i != 8)
    p.data[j++] = reinterpret_cast<char*>(&(tmpd))[i];
  tmpd = entity->getSpeedY();
  i = -1;
  while (++i != 8)
    p.data[j++] = reinterpret_cast<char*>(&(tmpd))[i];
  tmpi = entity->getLife();
  i = -1;
  while (++i != 4)
    p.data[j++] = reinterpret_cast<char*>(&(tmpi))[i];
  tmps = entity->getSpriteRepo();
  i = -1;
  while (tmps[++i] != 0)
    p.data[j++] = tmps[i];
  p.data[j] = 0;
  p.dataLength = j;
  i = -1;
  while (++i != p.dataLength)
    ret[i] = reinterpret_cast<char *>(&p)[i];
  ret[i] = 0;
  return (ret);
}

char		*Serialize::serialize(ICommand *cmd)
{
  packet	p;
  std::string	tmp;
  int		i;
  char		*ret;

  ret = new char[65471];
  p.dataType = cmd->getCommandName();
  p.cmdType = cmd->getCommandType();
  tmp = cmd->getCommandArg();
  i = -1;
  while (tmp[++i] != 0)
    p.data[i] = tmp[i];
  p.data[i] = 0;
  p.dataLength = tmp.size() + 8;
  i = -1;
  while (++i != p.dataLength)
    ret[i] = reinterpret_cast<char *>(&p)[i];
  ret[i] = 0;
  return (ret);
}

IEntity		*Serialize::unserializeEntity(char *data)
{
  (void)data;
  return (NULL);
}

#include	<iostream>
ICommand	*Serialize::unserializeCommand(char *data)
{
  packet	p;
  ICommand	*res;

  p = *reinterpret_cast<packet*>(data);
  std::cout << p.data << std::endl;
  switch (p.dataType)
    {
    case CHAT_INFO:
      break;
    case ROOM_INFO:
      break;
    case BASIC_CMD:
      res = new BasicCmd();
      res->setCommandArg(p.data);
      res->setCommandType(static_cast<CmdType>(p.cmdType));
      return (res);
      break;
    case ROOM_LIST:
      break;
    case ENTITY:
      break;
    case INPUT_CMD:
      break;
    }
  return (NULL);
}
