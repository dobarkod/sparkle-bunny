import socket
import serial
import time


def internet_reachable(host="8.8.8.8", port=53, timeout=3):
    """
    Host: 8.8.8.8 (google-public-dns-a.google.com)
    OpenPort: 53/tcp
    Service: domain (DNS/TCP)
    """
    try:
        socket.setdefaulttimeout(timeout)
        socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect((host, port))
        return True
    except Exception as ex:
        print ex.message
        return False


if __name__ == "__main__":
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    time.sleep(10)

    has_connection = True

    while True:
        if not internet_reachable():
            if has_connection:
                has_connection = False
                print 'PINGPONG'
                ser.write("PINGPONG:200")
        else:
            if not has_connection:
                has_connection = True
                print 'WHITELIGHT'
                ser.write("WHITELIGHT:1")
        time.sleep(5)

