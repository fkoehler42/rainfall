#include <cstring>
#include <cstdlib>

class N {

	public:
		char	annotation[100];
		int		val;

		N(int const v);
		void	setAnnotation(char *str);
		N		operator+(N const &rhs);

};

N::N(int const v) { this->val = v; return; }

void	N::setAnnotation(char *str) {
	memcpy(this->annotation, str, strlen(str));
}

N		N::operator+(N const &rhs) {
	return (N(this->val + rhs.val));
}

int		main(int ac, char **av) {
	if (ac <= 1)
		exit(1);

	N*	n1 = new N(5);
	N*	n2 = new N(6);

	n1->setAnnotation(av[1]);
	*n1 + *n2;
}