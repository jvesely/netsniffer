/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
template <class T>
class Singleton
{
	public:

	static T& instance() {
		static T inst;
		return inst;
	}
};
