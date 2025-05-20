#pragma once

class ObjectManager : public JobSerializer
{
public:
	ObjectManager();

	void Init();
	
	template<typename T, typename... Args>
	shared_ptr<T> CreateObject(Args&&... args)
	{
		auto id = m_idGenerator.fetch_add(1);
		auto object = MakeShared<T>(id, forward<Args>(args)...);
		m_objects.insert({ id, object });
		return object;
	}
	shared_ptr<class GameObject> FindById(unsigned long long id);

	void HandleEnterGame(Session* session, Protocol::REQ_ENTER pkt);

private:
	atomic<unsigned long long> m_idGenerator;
	concurrent_unordered_map<unsigned long long, shared_ptr<class GameObject>> m_objects;
};

