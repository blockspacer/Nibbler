#ifndef IMODULE_HPP
# define IMODULE_HPP

class IModule
{
public:

	virtual				~IModule(void) { }

	virtual void		disable(void) = 0;
	virtual void		enable(void) = 0;

	virtual void		handleEvents(void) = 0;
	virtual void		render(void) = 0;

};

#endif
