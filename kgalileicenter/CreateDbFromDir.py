
#! /usr/bin/env python

import sys, os, string, MySQLdb





# Prog to create a new database from a directory containing categories and files
#---------------------------------------------------------------------------------------------------
def main():
	print("cc")
	global database
	global usr
	global hst
	global password
	global lang
	global mime
	global cur

	global documentid
        global dirI
	global profondeurDir

	profondeurDir=2

	database=sys.argv[1]
	hst=sys.argv[2]
	usr=sys.argv[3]
	password=sys.argv[4]
	lang=sys.argv[5]
	mime=sys.argv[6]
        dirI=sys.argv[7]

	if password=="no" :
		password=""

	documentid=0


	print("cc")
	connect()
	parse(dirI)
	createprofiles()

#---------------------------------------------------------------------------------------------------
def parse(dir_to_treat):
	catId=0
	found=0

	dirnames = os.listdir(dir_to_treat)
	for dirname in dirnames :
		#print ("Processing directory"), dirname
		fulldirname=os.path.join(dir_to_treat,dirname)
		if os.path.isdir(fulldirname):
			#print "creation Cat parent 0"
			found=1
			catId = createcategory(dirname, 0, lang)
			parseDir(fulldirname,catId,1)

	if found== 0 :
		print("error - no category detected")


#---------------------------------------------------------------------------------------------------
def parseDir(dir_to_treat,parent,level):
	global profondeurDir
	catId=0

	dirnames = os.listdir(dir_to_treat)
	for dirname in dirnames :
		fullname=os.path.join(dir_to_treat,dirname)
		if os.path.isdir(fullname):
			#print ("Processing directory "), dirname
			if level< profondeurDir :
				#print "level ",level ,"create cat"
				catId = createcategory(dirname, parent, lang)
			else :
				#print "level",level,"catid=parent"
				catId = parent
			parseDir(fullname,catId,level+1)
		elif os.path.isfile(fullname) and ( os.path.splitext(fullname)[1] == '.html' or os.path.splitext(fullname)[1] == '.htm' ):
			#print ("Processing file "), dirname
			#print "parent ", parent
			insertdocument(fullname,parent,lang)



#---------------------------------------------------------------------------------------------------
def insertdocument(document, subcatid,lang):
        global cur
	global mime
	global documentid
        SQL = "INSERT INTO htmls set html='"+document+"', updated='2000-01-01', mimetype='"+mime+"', title='"+document+"', langid='"+lang+"'"
	cur.execute(SQL)
	documentid=documentid+1
        SQL = "INSERT INTO topicsbyhtmls SET topicid=%d , htmlid=%d " %(subcatid,documentid)
	cur.execute(SQL)




	# Prog to create a new database from a directory containing categories and files
#---------------------------------------------------------------------------------------------------
def createcategory(category,parentid,lg):
        global cur
	#global catid
	#global subcatid


	SQL = "SELECT topicid FROM topics WHERE name ='"+category+"' and parent=%d" %(parentid)
	cur.execute(SQL)
	id = cur.fetchone()
	if not id :
#                SQL = "INSERT INTO topics SET name='"+category+"',parent=%d ,langid='"+lang+"' " %(parentid)
		SQL = "INSERT INTO topics SET name='"+category+"',langid='"+lg+"', parent=%d  " %(parentid)
		cur.execute(SQL)
		SQL = "SELECT topicid FROM topics WHERE name ='"+category+"' and parent=%d" %(parentid)
		cur.execute(SQL)
		id = cur.fetchone()

	id =id[0]
	return id


#---------------------------------------------------------------------------------------------------
def connect():
	global cur
	global database
	global hst
	global password
	global usr

	cur = MySQLdb.connect(db=database,host=hst, user=usr,passwd=password ).cursor()




#---------------------------------------------------------------------------------------------------
def createprofiles():
        global cur

	print "Processing Creation of profiles ..."

	profileid=0
	langs=["en","fr"]
	SQL = "SELECT count(*) FROM users"
	cur.execute(SQL)
	nbuser=cur.fetchone()[0]
	users=xrange(1,nbuser+1)

	SQL = "SELECT * FROM topics where parent !=0"
	cur.execute(SQL)
	results=cur.fetchall()
	for res in results:
	        SQL = "SELECT name FROM topics WHERE topicid=%d" %(res[2])
		cur.execute(SQL)
		name=cur.fetchone()[0]
		profilename=name+"/"+res[1]
		for user in users :
		        SQL = "INSERT INTO profiles SET description='"+profilename+"', updated='2000-01-01', userid=%d, topicid=%d "%(user,res[0])
			cur.execute(SQL)
			profileid=profileid+1
			for lang in langs:
			        SQL= "INSERT INTO subprofiles SET langid='"+lang+"', attached='2001-01-01', profileid=%d" %(profileid)
				cur.execute(SQL)









if __name__== '__main__' :
        main()
