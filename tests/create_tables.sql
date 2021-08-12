CREATE TABLE IF NOT EXISTS room_registrant
(
    reg_uid         INT unsigned    NOT NULL AUTO_INCREMENT,
    reg_uname       CHAR(64)        NOT NULL,
    reg_use         TEXT            NOT NULL,
    reg_people      INT unsigned    NOT NULL,
    reg_contact     CHAR(11)        NOT NULL,
    reg_date        DATETIME        NOT NULL,
    reg_remark      TEXT            NULL,
    PRIMARY KEY(reg_uid)
) ENGINE = InnoDB;


CREATE TABLE IF NOT EXISTS room_checkedin
(
    checkedin_date  DATETIME        NOT NULL,
    reg_uid         INT UNSIGNED    NOT NULL
) ENGINE = InnoDB;

-- add foreign keys
ALTER TABLE room_checkedin
    ADD CONSTRAINT fk_cki_rgtant FOREIGN KEY (reg_uid) REFERENCES room_registrant (reg_uid);
