USE sensordata;
CREATE TABLE IF NOT EXISTS Data(
    ID int(11) NOT NULL AUTO_INCREMENT,
    TS timestamp NOT NULL,
    Temperature float(3,1) NOT NULL,
    Humidity float(3,1) NOT NULL,
    Luminosity float(6,2) NOT NULL,
    Height float(4,2) NOT NULL, 
    PRIMARY KEY(ID)
)engine=innodb AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;

