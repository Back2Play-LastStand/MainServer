#pragma once

class ObjectManager : public JobSerializer
{
public:
	ObjectManager();

	void Init();
	
	template<typename T, typename... Args>
	shared_ptr<T> CreateObject(Args&&... args)
	{
		auto object = MakeShared<T>(m_idGenerator.fetch_add(1),forward<Args>(args)...);
		m_objects.insert({ m_idGenerator, object });
		return object;
	}

	void HandleEnterGame(Session* session, Protocol::REQ_ENTER pkt);

private:
	atomic<unsigned long long> m_idGenerator;
	concurrent_unordered_map<unsigned long long, shared_ptr<class GameObject>> m_objects;
};

