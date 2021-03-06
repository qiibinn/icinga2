/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2018 Icinga Development Team (https://icinga.com/)      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef SOCKET_H
#define SOCKET_H

#include "base/i2-base.hpp"
#include "base/object.hpp"
#include <boost/thread/mutex.hpp>

namespace icinga
{

/**
 * Base class for connection-oriented sockets.
 *
 * @ingroup base
 */
class Socket : public Object
{
public:
	DECLARE_PTR_TYPEDEFS(Socket);

	Socket() = default;
	Socket(SOCKET fd);
	~Socket() override;

	SOCKET GetFD() const;

	void Close();

	std::pair<String, String> GetClientAddressDetails();
	String GetClientAddress();
	std::pair<String, String> GetPeerAddressDetails();
	String GetPeerAddress();

	size_t Read(void *buffer, size_t size);
	size_t Write(const void *buffer, size_t size);

	void Listen();
	Socket::Ptr Accept();

	bool Poll(bool read, bool write, struct timeval *timeout = nullptr);

	void MakeNonBlocking();

	static void SocketPair(SOCKET s[2]);

protected:
	void SetFD(SOCKET fd);

	int GetError() const;

	mutable boost::mutex m_SocketMutex;

private:
	SOCKET m_FD{INVALID_SOCKET}; /**< The socket descriptor. */

	static std::pair<String, String> GetDetailsFromSockaddr(sockaddr *address, socklen_t len);
	static String GetHumanReadableAddress(const std::pair<String, String>& socketDetails);
};

class socket_error : virtual public std::exception, virtual public boost::exception { };

}

#endif /* SOCKET_H */
