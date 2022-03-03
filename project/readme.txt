node
	data
	adj => sibling
	next => parent/child

file 
	data => fileName pointer
	adj => sibling
	next => parent

folder
	data => fileName pointer
	adj => sibling
	next => child

Functions: 
	Add file: touch "fileName"
		CreateNode
		AddNode
	
	Add folder: mkdir "Folder"
		CreateNode
		AddNode
	
	write File:
		
	
	Delete file: rm "Node"
		deleteFile
		removeNode
	
	Delete folder rm "Node"
		remove node 
	
	Move dir: cd "dir" / cd ".."
		changeDir / returnDir
	
	findDir

	
			


	