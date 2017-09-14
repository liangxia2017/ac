CREATE TABLE "portal_user" (
"id" INTEGER,
"username" TEXT,
"password" TEXT,
"register_time" TEXT,
"sign_in_time" TEXT,
"user_ip" TEXT,
"user_host" TEXT,
"user_agent" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "username" UNIQUE ("username")
);

CREATE UNIQUE INDEX "username" ON "portal_user" ("username" );

